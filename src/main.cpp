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
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// Définir les broches UART pour le capteur GT215
#define FINGERPRINT_RX 16
#define FINGERPRINT_TX 17

// Définir les broches SPI pour la carte SD
#define SD_MOSI 23
#define SD_MISO 19
#define SD_CLK 18
#define SD_CS 5

// Initialiser le capteur d'empreintes
HardwareSerial fingerprintSerial(2);
Adafruit_Fingerprint finger(&fingerprintSerial);

// Configuration du point d'accès Wi-Fi
const char* ssid = "ESP32-Dashboard";
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

// Fonction pour lister les fichiers sur la carte SD
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

// Fonction pour initialiser la carte SD
void setupSDCard() {
    if (!SD.begin(SD_CS)) {
        Serial.println("Erreur : Impossible de monter la carte SD !");
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("Aucune carte SD détectée !");
        return;
    }
    Serial.print("Type de carte SD : ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("Inconnu");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("Taille de la carte SD : %lluMB\n", cardSize);
    listDir(SD, "/", 0);
}

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
    } else {
        Serial.println("Erreur : Impossible d'initialiser le capteur d'empreintes !");
        while (1);
    }
}

// Fonction pour vérifier les empreintes digitales
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

void setup() {
    Serial.begin(115200);
    Wire.begin();

    servo1.attach(servoPins[0]);
    servo2.attach(servoPins[1]);
    servo3.attach(servoPins[2]);
    servo4.attach(servoPins[3]);

    pinMode(buzzerPin, OUTPUT);
    deactivateBuzzer();

    pinMode(irSensorPin, INPUT);
    pinMode(irLedPin, OUTPUT);
    digitalWrite(irLedPin, LOW);

    setupDisplay();

    if (!LittleFS.begin()) {
        Serial.println("Erreur : Impossible de monter LittleFS !");
        return;
    }

    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    setupWebServer();
    setupFingerprintSensor();
    setupSDCard(); // Initialiser la carte SD
}

void loop() {
    // Boucle principale
}