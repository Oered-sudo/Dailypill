#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h> // Assurez-vous que la bibliothèque est installée
#include "Display.h"
#include "AlarmManager.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP-DASH.h>

// Configuration du point d'accès
const char* ssid = "ESP32-Dashboard"; // Nom du réseau Wi-Fi
const char* password = "12345678"; // Mot de passe (au moins 8 caractères)

// Broche du buzzer
const int buzzerPin = 25;

// Configuration des servomoteurs
Servo servo1, servo2, servo3, servo4; // Déclaration des objets Servo
const int servoPins[] = {26, 27, 14, 12}; // Broches des servos

// Configuration du capteur PIR
const int pirPin = 33;

// Gestionnaire d'alarmes
AlarmManager alarmManager;

// Serveur Web et tableau de bord
AsyncWebServer server(80);
Dashboard dashboard(&server);

// Widgets
Card temperatureCard("Température", "°C");
Card humidityCard("Humidité", "%");
Button toggleButton("Activer/Désactiver", true);

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

    // Configuration des widgets
    dashboard.addCard(temperatureCard);
    dashboard.addCard(humidityCard);
    dashboard.addButton(toggleButton);

    // Gestionnaire pour le bouton
    toggleButton.attachCallback([](bool state) {
        Serial.printf("Bouton activé : %s\n", state ? "ON" : "OFF");
    });

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

    // Simule des données pour les widgets
    static float temperature = 25.0;
    static float humidity = 50.0;

    temperature += random(-10, 10) * 0.1;
    humidity += random(-5, 5) * 0.1;

    temperatureCard.update(temperature);
    humidityCard.update(humidity);

    delay(1000);
}