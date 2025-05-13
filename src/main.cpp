#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SSD1306Wire.h>
#include <LittleFS.h> // Inclure LittleFS
#include "AlarmManager.h"
#include <Adafruit_Fingerprint.h> // Bibliothèque pour le capteur d'empreintes

// Définir les broches UART pour le capteur GT215
#define FINGERPRINT_RX 16
#define FINGERPRINT_TX 17

// Initialiser le capteur d'empreintes
HardwareSerial fingerprintSerial(2); // Utiliser UART2
Adafruit_Fingerprint finger(&fingerprintSerial);

// Configuration du point d'accès Wi-Fi
const char* ssid = "ESP32-Dashboard";
const char* password = "12345678";

// Broches des périphériques
const int buzzerPin = 25;
const int irSensorPin = 33;
const int irLedPin = 32;

// Configuration des servomoteurs
Servo servo1, servo2, servo3, servo4;
const int servoPins[] = {26, 27, 14, 12};

// Gestionnaire d'alarmes
AlarmManager alarmManager;

// Serveur Web
AsyncWebServer server(80);

// Variables globales
bool alarmActive = false;
int selectedServo = 0;
String alarmTime = "00:00";

// Écran OLED
SSD1306Wire display(0x3C, SDA, SCL);

// Fonction pour activer le buzzer et effectuer une rotation du servo
void activateBuzzer() {
    digitalWrite(buzzerPin, HIGH);
    display.clear();
    display.drawString(0, 0, "Alarme activée !");
    display.display();

    // Effectuer 2,5 rotations (900°) avec le servo sélectionné
    Servo* servos[] = {&servo1, &servo2, &servo3, &servo4};
    if (selectedServo >= 0 && selectedServo < 4) {
        Servo* servo = servos[selectedServo];
        for (int i = 0; i < 5; i++) { // 5 demi-tours
            for (int angle = 0; angle <= 180; angle += 10) {
                servo->write(angle);
                delay(15);
            }
            for (int angle = 180; angle >= 0; angle -= 10) {
                servo->write(angle);
                delay(15);
            }
        }
        // Revenir à la position initiale
        servo->write(0);
    }
}

// Fonction pour désactiver le buzzer
void deactivateBuzzer() {
    digitalWrite(buzzerPin, LOW);
    display.clear();
    display.drawString(0, 0, "Alarme désactivée !");
    display.display();
}

// Fonction pour envoyer un signal IR
void sendIrSignal() {
    digitalWrite(irLedPin, HIGH);
    delay(100);
    digitalWrite(irLedPin, LOW);
    Serial.println("Signal IR envoyé !");
    display.clear();
    display.drawString(0, 0, "Signal IR envoyé !");
    display.display();
}

// Fonction pour faire tourner un servo spécifique
void rotateServo(int servoIndex) {
    Servo* servos[] = {&servo1, &servo2, &servo3, &servo4};
    if (servoIndex >= 0 && servoIndex < 4) {
        for (int angle = 0; angle <= 180; angle += 10) {
            servos[servoIndex]->write(angle);
            delay(15);
        }
        for (int angle = 180; angle >= 0; angle -= 10) {
            servos[servoIndex]->write(angle);
            delay(15);
        }
        display.clear();
        display.drawString(0, 0, "Servo activé !");
        display.display();
    }
}

// Fonction pour configurer l'écran OLED
void setupDisplay() {
    display.init();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "ESP32 Dashboard");
    display.display();
}

// Fonction pour configurer le serveur web
void setupWebServer() {
    // Route principale pour servir l'interface utilisateur
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html"); // Utiliser LittleFS
    });

    // Route pour activer/désactiver l'alarme
    server.on("/toggleAlarm", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (alarmActive) {
            deactivateBuzzer();
            alarmActive = false;
        } else {
            activateBuzzer();
            alarmActive = true;
        }
        request->send(200, "text/plain", "OK");
    });

    // Route pour sélectionner un étage
    server.on("/selectServo", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("servo", true)) {
            selectedServo = request->getParam("servo", true)->value().toInt();
            rotateServo(selectedServo);
        }
        request->send(200, "text/plain", "OK");
    });

    // Route pour envoyer un signal IR
    server.on("/sendIrSignal", HTTP_POST, [](AsyncWebServerRequest *request) {
        sendIrSignal();
        request->send(200, "text/plain", "OK");
    });

    // Route pour configurer l'heure de l'alarme
    server.on("/setAlarm", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("time", true)) {
            alarmTime = request->getParam("time", true)->value();
            Serial.println("Alarme configurée à : " + alarmTime);
            display.clear();
            display.drawString(0, 0, "Alarme configurée :");
            display.drawString(0, 20, alarmTime);
            display.display();
        }
        request->send(200, "text/plain", "OK");
    });

    // Démarrer le serveur
    server.begin();
}

void setupFingerprintSensor() {
    fingerprintSerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);
    finger.begin(57600); // Fournir la vitesse en bauds ici
    if (finger.verifyPassword()) {
        Serial.println("Capteur d'empreintes initialisé !");
    } else {
        Serial.println("Erreur : Impossible d'initialiser le capteur d'empreintes !");
        while (1); // Bloquer si le capteur ne fonctionne pas
    }
}

bool verifyFingerprint() {
    Serial.println("Placez votre doigt sur le capteur...");
    int result = finger.getImage();
    if (result != FINGERPRINT_OK) {
        Serial.println("Aucune empreinte détectée !");
        return false;
    }

    result = finger.image2Tz();
    if (result != FINGERPRINT_OK) {
        Serial.println("Erreur lors de la conversion de l'image !");
        return false;
    }

    result = finger.fingerFastSearch();
    if (result == FINGERPRINT_OK) {
        Serial.print("Empreinte reconnue, ID : ");
        Serial.println(finger.fingerID);
        return true;
    } else {
        Serial.println("Empreinte non reconnue !");
        return false;
    }
}

bool isCupPresent() {
    int sensorValue = digitalRead(irSensorPin);
    if (sensorValue == HIGH) { // Si le capteur détecte un objet
        Serial.println("Tasse détectée !");
        display.clear();
        display.drawString(0, 0, "Tasse détectée !");
        display.display();
        return true;
    } else {
        Serial.println("Aucune tasse détectée !");
        display.clear();
        display.drawString(0, 0, "Aucune tasse !");
        display.display();
        return false;
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialisation des servomoteurs
    servo1.attach(servoPins[0]);
    servo2.attach(servoPins[1]);
    servo3.attach(servoPins[2]);
    servo4.attach(servoPins[3]);

    // Initialisation du buzzer
    pinMode(buzzerPin, OUTPUT);
    deactivateBuzzer();

    // Initialisation du capteur IR et de la LED IR
    pinMode(irSensorPin, INPUT);
    pinMode(irLedPin, OUTPUT);
    digitalWrite(irLedPin, LOW);

    // Configuration de l'écran OLED
    setupDisplay();

    // Initialisation de LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Erreur : Impossible de monter LittleFS !");
        return;
    }

    // Configuration du Wi-Fi
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    // Configuration du serveur web
    setupWebServer();

    // Configuration du capteur d'empreintes
    setupFingerprintSensor();
}

void loop() {
    // Vérifie si l'heure actuelle correspond à l'heure de l'alarme
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);
    char currentTime[6];
    strftime(currentTime, sizeof(currentTime), "%H:%M", timeInfo);

    if (alarmActive && String(currentTime) == alarmTime) {
        // Vérifie si une tasse est présente avant d'activer le buzzer
        if (isCupPresent()) {
            activateBuzzer();
        } else {
            Serial.println("Aucune tasse détectée, alarme désactivée !");
            deactivateBuzzer();
        }
    }

    // Vérifie les empreintes digitales
    if (verifyFingerprint()) {
        Serial.println("Empreinte reconnue : accès autorisé !");
        // Ajoutez ici les actions à effectuer en cas de succès
        deactivateBuzzer(); // Exemple : désactiver l'alarme si l'empreinte est reconnue
    } else {
        Serial.println("Empreinte non reconnue ou absente.");
    }

    delay(1000); // Attendre avant la prochaine vérification
}