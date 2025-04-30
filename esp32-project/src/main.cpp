#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SSD1306Wire.h>
#include "AlarmManager.h"

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

// Fonction pour activer le buzzer
void activateBuzzer() {
    digitalWrite(buzzerPin, HIGH);
    display.clear();
    display.drawString(0, 0, "Alarme activée !");
    display.display();
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
        request->send(SPIFFS, "/index.html", "text/html");
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

    // Configuration du Wi-Fi
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    // Configuration du serveur web
    setupWebServer();
}

void loop() {
    // Vérifie si l'heure actuelle correspond à l'heure de l'alarme
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);
    char currentTime[6];
    strftime(currentTime, sizeof(currentTime), "%H:%M", timeInfo);

    if (alarmActive && String(currentTime) == alarmTime) {
        activateBuzzer();
    }

    delay(1000);
}