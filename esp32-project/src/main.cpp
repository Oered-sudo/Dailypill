#include <Arduino.h>
#include <Wire.h>
#include <Servo.h> // Bibliothèque pour contrôler les servomoteurs
#include "i2c_display.h"
#include "alarm_manager.h"
#include <WiFi.h>
#include <WebServer.h>

// Configuration du point d'accès
const char* ssid = "ESP32-Alarm"; // Nom du réseau Wi-Fi
const char* password = "12345678"; // Mot de passe (au moins 8 caractères)

// Création du serveur web sur le port 80
WebServer server(80);

// Broche du buzzer
const int buzzerPin = 25; // Remplacez 25 par la broche que vous utilisez pour le buzzer

// Configuration des servomoteurs
Servo servo1, servo2, servo3, servo4;
const int servoPins[] = {26, 27, 14, 12}; // Broches des servomoteurs

// Configuration du capteur PIR
const int pirPin = 33; // Broche du capteur PIR

// Page HTML à servir
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Alarm Manager</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; background-color: #f4f4f9; }
        h1 { background-color: #4CAF50; color: white; padding: 10px; }
        form { margin: 20px auto; max-width: 300px; }
        input, button { padding: 10px; margin: 5px; width: 100%; }
        button { background-color: #4CAF50; color: white; border: none; cursor: pointer; }
        button:hover { background-color: #45a049; }
    </style>
</head>
<body>
    <h1>ESP32 Alarm Manager</h1>
    <form action="/set_alarm" method="GET">
        <label for="name">Alarm Name:</label><br>
        <input type="text" id="name" name="name" required><br>
        <label for="time">Time:</label><br>
        <input type="time" id="time" name="time" required><br>
        <button type="submit">Set Alarm</button>
    </form>
</body>
</html>
)rawliteral";

// Fonction pour gérer la page principale
void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

// Fonction pour gérer la configuration des alarmes
void handleSetAlarm() {
    String name = server.arg("name");
    String time = server.arg("time");

    // Logique pour gérer l'alarme (à implémenter)
    Serial.printf("Alarm set: %s at %s\n", name.c_str(), time.c_str());

    // Réponse au client
    server.send(200, "text/html", "<h1>Alarme configurée avec succès !</h1><a href='/'>Retour</a>");
}

// Fonction pour activer le buzzer
void activateBuzzer() {
    digitalWrite(buzzerPin, HIGH); // Activer le buzzer
}

// Fonction pour désactiver le buzzer
void deactivateBuzzer() {
    digitalWrite(buzzerPin, LOW); // Désactiver le buzzer
}

// Fonction pour faire tourner les servomoteurs
void rotateServos() {
    for (int i = 0; i < 2; i++) { // Effectuer 2 rotations
        for (int angle = 0; angle <= 180; angle += 10) { // Rotation de 0 à 180 degrés
            servo1.write(angle);
            servo2.write(angle);
            servo3.write(angle);
            servo4.write(angle);
            delay(15);
        }
        for (int angle = 180; angle >= 0; angle -= 10) { // Retour de 180 à 0 degrés
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
    Wire.begin(); // Initialiser I2C
    initDisplay(); // Initialiser l'écran I2C
    initAlarmManager(); // Initialiser le gestionnaire d'alarmes

    // Configuration du buzzer
    pinMode(buzzerPin, OUTPUT);
    deactivateBuzzer(); // Assurez-vous que le buzzer est désactivé au démarrage

    // Configuration des servomoteurs
    servo1.attach(servoPins[0]);
    servo2.attach(servoPins[1]);
    servo3.attach(servoPins[2]);
    servo4.attach(servoPins[3]);

    // Configuration du capteur PIR
    pinMode(pirPin, INPUT);

    // Configuration du point d'accès
    WiFi.softAP(ssid, password);
    Serial.println("Point d'accès configuré !");
    Serial.print("Nom du réseau : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.softAPIP());

    // Configuration des routes du serveur web
    server.on("/", handleRoot);
    server.on("/set_alarm", handleSetAlarm);

    // Démarrage du serveur
    server.begin();
    Serial.println("Serveur web démarré !");
}

void loop() {
    checkAlarms(); // Vérifier les alarmes programmées

    // Si une alarme est active, activer le buzzer
    if (isAlarmActive()) { // Remplacez par la fonction qui vérifie si une alarme est déclenchée
        activateBuzzer();
    } else {
        deactivateBuzzer();

        // Si le capteur PIR détecte un mouvement, faire tourner les servos
        if (digitalRead(pirPin) == HIGH) {
            Serial.println("Mouvement détecté !");
            rotateServos();
        }
    }

    displayAlarms(); // Afficher les alarmes sur l'écran
    delay(1000); // Attendre 1 seconde avant la prochaine vérification

    // Gestion des requêtes client
    server.handleClient();
}