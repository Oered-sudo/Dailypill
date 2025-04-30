#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <SSD1306Wire.h>
#include "AlarmManager.h"

// Configuration du point d'accès Wi-Fi
const char* ssid = "ESP32-Dashboard";
const char* password = "12345678";

// Broches des périphériques
const int buzzerPin = 25;
const int irSensorPin = 33;
const int irLedPin = 32; // Nouvelle broche pour la LED IR

// Configuration des servomoteurs
Servo servo1, servo2, servo3, servo4;
const int servoPins[] = {26, 27, 14, 12};

// Gestionnaire d'alarmes
AlarmManager alarmManager;

// Serveur Web et tableau de bord
AsyncWebServer server(80);
Dashboard dashboard(&server);

// Widgets pour l'interface web
Card alarmCard("Alarmes", "Actives");
Select servoSelect("Sélectionner l'étage");
Button fingerprintButton("Activer avec empreinte", true);
Button alarmToggleButton("Activer/Désactiver l'alarme", true);

// Variables globales
bool alarmActive = false;
int selectedServo = 0;

// Écran OLED
SSD1306Wire display(0x3C, SDA, SCL); // Adresse I2C par défaut pour SSD1306

// Fonction pour activer le buzzer
void activateBuzzer() {
    digitalWrite(buzzerPin, HIGH);
}

// Fonction pour désactiver le buzzer
void deactivateBuzzer() {
    digitalWrite(buzzerPin, LOW);
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

// Fonction pour mettre à jour l'écran OLED
void updateDisplay(const String& message) {
    display.clear();
    display.drawString(0, 0, message);
    display.display();
}

// Fonction pour configurer l'interface web
void setupDashboard() {
    // Ajouter les widgets au tableau de bord
    dashboard.addCard(alarmCard);
    dashboard.addSelect(servoSelect);
    dashboard.addButton(fingerprintButton);
    dashboard.addButton(alarmToggleButton);

    // Ajouter des options pour sélectionner l'étage
    servoSelect.addOption("Étage 1", "0");
    servoSelect.addOption("Étage 2", "1");
    servoSelect.addOption("Étage 3", "2");
    servoSelect.addOption("Étage 4", "3");

    // Gestionnaire pour le bouton d'empreinte digitale
    fingerprintButton.attachCallback([](bool state) {
        if (state) {
            Serial.println("Empreinte validée !");
            updateDisplay("Empreinte validée !");
            rotateServo(selectedServo);
        }
    });

    // Gestionnaire pour le bouton d'alarme
    alarmToggleButton.attachCallback([](bool state) {
        alarmActive = state;
        if (alarmActive) {
            Serial.println("Alarme activée !");
            updateDisplay("Alarme activée !");
            activateBuzzer();
        } else {
            Serial.println("Alarme désactivée !");
            updateDisplay("Alarme désactivée !");
            deactivateBuzzer();
        }
    });

    // Gestionnaire pour la sélection de l'étage
    servoSelect.attachCallback([](String value) {
        selectedServo = value.toInt();
        String message = "Étage sélectionné : " + String(selectedServo + 1);
        Serial.println(message);
        updateDisplay(message);
    });
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

    // Initialisation du capteur IR et de la LED
    pinMode(irSensorPin, INPUT);
    pinMode(irLedPin, OUTPUT);
    digitalWrite(irLedPin, LOW); // Assurez-vous que la LED est éteinte au démarrage

    // Configuration de l'écran OLED
    setupDisplay();

    // Configuration du Wi-Fi
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    // Configuration de l'interface web
    setupDashboard();

    // Démarrage du serveur
    server.begin();
}

void loop() {
    // Vérifie les alarmes actives
    alarmManager.checkAlarms();

    // Si une alarme est active, activer le buzzer
    if (alarmManager.isAlarmActive()) {
        activateBuzzer();
    } else {
        deactivateBuzzer();
    }

    // Vérifie si le capteur IR détecte une réception
    if (digitalRead(irSensorPin) == HIGH) {
        Serial.println("Réception détectée !");
        updateDisplay("Réception détectée !");
        digitalWrite(irLedPin, HIGH); // Allume la LED
    } else {
        digitalWrite(irLedPin, LOW); // Éteint la LED
    }

    delay(1000);
}