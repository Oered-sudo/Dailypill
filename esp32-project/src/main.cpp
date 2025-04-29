#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include "Display.h"
#include "AlarmManager.h"
#include <WiFi.h>
#include <ESPUI.h>

// Configuration du point d'accès
const char* ssid = "ESP32-Alarm"; // Nom du réseau Wi-Fi
const char* password = "12345678"; // Mot de passe (au moins 8 caractères)

// Broche du buzzer
const int buzzerPin = 25;

// Configuration des servomoteurs
Servo servo1, servo2, servo3, servo4;
const int servoPins[] = {26, 27, 14, 12};

// Configuration du capteur PIR
const int pirPin = 33;

// Gestionnaire d'alarmes
AlarmManager alarmManager;

// Fonction pour activer le buzzer
void activateBuzzer() {
    digitalWrite(buzzerPin, HIGH);
}

// Fonction pour désactiver le buzzer
void deactivateBuzzer() {
    digitalWrite(buzzerPin, LOW);
}

// Fonction pour faire tourner les servomoteurs
void rotateServos() {
    for (int i = 0; i < 2; i++) {
        for (int angle = 0; angle <= 180; angle += 10) {
            servo1.write(angle);
            servo2.write(angle);
            servo3.write(angle);
            servo4.write(angle);
            delay(15);
        }
        for (int angle = 180; angle >= 0; angle -= 10) {
            servo1.write(angle);
            servo2.write(angle);
            servo3.write(angle);
            servo4.write(angle);
            delay(15);
        }
    }
}

// Callback pour ajouter une alarme via ESPUI
void addAlarmCallback(Control *sender, int type) {
    if (type == B_UP) {
        String alarmName = ESPUI.getControl(1)->value;
        String alarmTime = ESPUI.getControl(2)->value;

        int hour = alarmTime.substring(0, 2).toInt();
        int minute = alarmTime.substring(3, 5).toInt();

        alarmManager.addAlarm(alarmName, hour, minute);
        Serial.printf("Alarme ajoutée : %s à %02d:%02d\n", alarmName.c_str(), hour, minute);
    }
}

// Initialisation de l'interface ESPUI
void setupESPUI() {
    ESPUI.begin("ESP32 Alarm Manager");

    // Champ pour le nom de l'alarme
    ESPUI.addControl(ControlType::Text, "Nom de l'alarme", "", ControlColor::Turquoise, 1);

    // Champ pour l'heure de l'alarme
    ESPUI.addControl(ControlType::Text, "Heure (HH:MM)", "", ControlColor::Turquoise, 2);

    // Bouton pour ajouter une alarme
    ESPUI.addControl(ControlType::Button, "Ajouter une alarme", "Ajouter", ControlColor::Emerald, 3, addAlarmCallback);
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    setupDisplay();

    // Initialisation des servomoteurs
    servo1.attach(servoPins[0]);
    servo2.attach(servoPins[1]);
    servo3.attach(servoPins[2]);
    servo4.attach(servoPins[3]);

    // Initialisation du buzzer
    pinMode(buzzerPin, OUTPUT);
    deactivateBuzzer();

    // Initialisation du capteur PIR
    pinMode(pirPin, INPUT);

    // Configuration du Wi-Fi
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    // Initialisation de l'interface ESPUI
    setupESPUI();
}

void loop() {
    // Vérifie les alarmes actives
    void checkAlarms() {
        alarmManager.checkAlarms();
    }
    
        checkAlarms();

    // Si une alarme est active, activer le buzzer
    if (isAlarmActive()) {
        activateBuzzer();
    } else {
        deactivateBuzzer();

        // Si le capteur PIR détecte un mouvement, faire tourner les servos
        if (digitalRead(pirPin) == HIGH) {
            Serial.println("Mouvement détecté !");
            rotateServos();
        }
    }

    // Mettre à jour l'affichage
    const auto& alarms = alarmManager.getAlarms();
    String alarmNames[alarms.size()];
    for (size_t i = 0; i < alarms.size(); i++) {
        alarmNames[i] = alarms[i].name;
    }
    updateDisplay(alarmNames, alarms.size());

    delay(1000);
}