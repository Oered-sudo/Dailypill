#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SSD1306Wire.h>
#include <LittleFS.h>
#include "AlarmManager.h"
#include <Adafruit_Fingerprint.h>

// Définir les broches UART pour le capteur GT215
#define FINGERPRINT_RX 16
#define FINGERPRINT_TX 17

// Initialiser le capteur d'empreintes
HardwareSerial fingerprintSerial(2);
Adafruit_Fingerprint finger(&fingerprintSerial);

// Configuration du point d'accès Wi-Fi
const char* ssid = "Dailypill-Dashboard";
const char* password = "12345678";

// Broches des périphériques
const int buzzerPin = 20;
const int irSensorPin = 22;
const int irLedPin = 21;

// Configuration des servomoteurs
Servo servo1, servo2, servo3, servo4;
const int servoPins[] = {25, 26, 27, 14}; // GPIO25, GPIO26, GPIO27, GPIO14

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
        servo->write(0); // Revenir à la position initiale
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

// Fonction pour vérifier la présence d'une tasse
bool isCupPresent() {
    int sensorValue = digitalRead(irSensorPin);
    if (sensorValue == HIGH) {
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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
    });

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

    server.on("/selectServo", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("servo", true)) {
            selectedServo = request->getParam("servo", true)->value().toInt();
            activateBuzzer();
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/sendIrSignal", HTTP_POST, [](AsyncWebServerRequest *request) {
        sendIrSignal();
        request->send(200, "text/plain", "OK");
    });

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

    server.begin();
}

// Fonction pour configurer le capteur d'empreintes
void setupFingerprintSensor() {
    fingerprintSerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);
    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Capteur d'empreintes initialisé !");
        display.clear();
        display.drawString(0, 0, "Capteur d'empreintes");
        display.drawString(0, 10, "initialise !");
        display.display();
    } else {
        Serial.println("Erreur : Impossible d'initialiser le capteur d'empreintes !");
        display.clear();
        display.drawString(0, 0, "Erreur: Capteur");
        display.drawString(0, 10, "non initialise !");
        display.display();
        while (1);
    }
}

// Fonction pour vérifier les empreintes digitales
bool verifyFingerprint() {
    Serial.println("Placez votre doigt sur le capteur...");
    display.clear();
    display.drawString(0, 0, "Placez votre doigt");
    display.drawString(0, 10, "sur le capteur...");
    display.display();

    int result = finger.getImage();
    if (result != FINGERPRINT_OK) {
        Serial.println("Aucune empreinte détectée !");
        display.clear();
        display.drawString(0, 0, "Aucune empreinte");
        display.drawString(0, 10, "detectee !");
        display.display();
        return false;
    }

    result = finger.image2Tz();
    if (result != FINGERPRINT_OK) {
        Serial.println("Erreur lors de la conversion de l'image !");
        display.clear();
        display.drawString(0, 0, "Erreur: Conversion");
        display.drawString(0, 10, "de l'image !");
        display.display();
        return false;
    }

    result = finger.fingerFastSearch();
    if (result == FINGERPRINT_OK) {
        Serial.print("Empreinte reconnue, ID : ");
        Serial.println(finger.fingerID);
        display.clear();
        display.drawString(0, 0, "Empreinte reconnue !");
        display.drawString(0, 10, "ID: " + String(finger.fingerID));
        display.display();
        return true;
    } else {
        Serial.println("Empreinte non reconnue !");
        display.clear();
        display.drawString(0, 0, "Empreinte non");
        display.drawString(0, 10, "reconnue !");
        display.display();
        return false;
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialiser les servomoteurs
    servo1.attach(servoPins[0]);
    servo2.attach(servoPins[1]);
    servo3.attach(servoPins[2]);
    servo4.attach(servoPins[3]);

    // Configurer les broches
    pinMode(buzzerPin, OUTPUT);
    deactivateBuzzer();

    pinMode(irSensorPin, INPUT);
    pinMode(irLedPin, OUTPUT);
    digitalWrite(irLedPin, LOW);

    // Initialiser l'écran OLED
    setupDisplay();

    // Vérifier le système de fichiers
    if (!LittleFS.begin()) {
        Serial.println("Erreur : Impossible de monter LittleFS !");
        display.clear();
        display.drawString(0, 0, "Erreur: LittleFS");
        display.drawString(0, 10, "Non monte !");
        display.display();
        while (1); // Bloquer le programme
    }

    // Configurer le Wi-Fi
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    // Afficher les informations Wi-Fi sur l'écran OLED
    display.clear();
    display.drawString(0, 0, "Wi-Fi configure !");
    display.drawString(0, 10, "SSID: " + String(ssid));
    display.drawString(0, 20, "IP: " + WiFi.softAPIP().toString());
    display.display();
    delay(3000); // Attendre 3 secondes pour que l'utilisateur voie les infos

    // Configurer le serveur web
    setupWebServer();

    // Configurer le capteur d'empreintes
    setupFingerprintSensor();

    // Afficher un message de démarrage
    display.clear();
    display.drawString(0, 0, "Systeme pret !");
    display.drawString(0, 10, "Placez votre doigt");
    display.drawString(0, 20, "puis placez une tasse.");
    display.display();
}

void loop() {
    static unsigned long lastAction = 0;
    static int state = 0;

    // Changer d'état toutes les 2 secondes
    if (millis() - lastAction > 2000) {
        state = (state + 1) % 3; // Passer à l'état suivant (0, 1, 2)
        lastAction = millis();
    }

    switch (state) {
        case 0: // Vérifier les empreintes digitales
            display.clear();
            display.drawString(0, 0, "Etat: Verification");
            display.drawString(0, 10, "Empreinte digitale...");
            display.display();

            if (!alarmActive) {
                activateBuzzer(); // Activer l'alarme si elle n'est pas déjà active
                alarmActive = true;
            }

            if (verifyFingerprint()) {
                Serial.println("Empreinte valide, désactivation de l'alarme !");
                deactivateBuzzer();
                alarmActive = false;

                display.clear();
                display.drawString(0, 0, "Empreinte valide !");
                display.drawString(0, 10, "Alarme desactivee.");
                display.display();
            } else {
                Serial.println("Empreinte non valide, alarme maintenue !");
                display.drawString(0, 20, "Empreinte non valide !");
                display.display();
            }
            break;

        case 1: // Vérifier la présence d'une tasse
            display.clear();
            display.drawString(0, 0, "Etat: Detection");
            display.drawString(0, 10, "Detection de tasse...");
            display.display();

            if (isCupPresent()) {
                Serial.println("Tasse détectée, envoi du signal IR !");
                sendIrSignal();

                display.clear();
                display.drawString(0, 0, "Tasse detectee !");
                display.drawString(0, 10, "Signal IR envoye.");
                display.display();
            } else {
                display.drawString(0, 20, "Aucune tasse detectee.");
                display.display();
            }
            break;

        case 2: // Vérifier toutes les conditions
            display.clear();
            display.drawString(0, 0, "Etat: Verification");
            display.drawString(0, 10, "Verification des conditions...");
            display.display();

            if (alarmActive && isCupPresent() && verifyFingerprint()) {
                Serial.println("Conditions remplies, effectuer une action.");
                Servo* servos[] = {&servo1, &servo2, &servo3, &servo4};
                if (selectedServo >= 0 && selectedServo < 4) {
                    Servo* servo = servos[selectedServo];
                    int initialPosition = servo->read(); // Lire la position initiale
                    servo->write(90);
                    delay(500); // Attendre un moment pour que le servo atteigne la position
                    servo->write(initialPosition); // Revenir à la position initiale
                }

                display.clear();
                display.drawString(0, 0, "Action effectuee !");
                display.drawString(0, 10, "Servo active.");
                display.display();
            } else {
                display.drawString(0, 20, "Conditions non remplies.");
                display.display();
            }
            break;

        default:
            Serial.println("État inconnu !");
            display.clear();
            display.drawString(0, 0, "Etat inconnu !");
            display.display();
            break;
    }

    delay(100); // Petite pause pour éviter une boucle trop rapide
}