#include <Arduino.h>
#include <Wire.h>
#include "i2c_display.h"
#include "alarm_manager.h"
#include <WiFi.h>
#include <WebServer.h>

// Configuration du point d'accès
const char* ssid = "ESP32-Alarm"; // Nom du réseau Wi-Fi
const char* password = "12345678"; // Mot de passe (au moins 8 caractères)

// Création du serveur web sur le port 80
WebServer server(80);

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

void setup() {
    Serial.begin(115200);
    Wire.begin(); // Initialiser I2C
    initDisplay(); // Initialiser l'écran I2C
    initAlarmManager(); // Initialiser le gestionnaire d'alarmes

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
    displayAlarms(); // Afficher les alarmes sur l'écran
    delay(1000); // Attendre 1 seconde avant la prochaine vérification

    // Gestion des requêtes client
    server.handleClient();
}