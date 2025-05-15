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

// === Déclarations globales ===

// Broches et périphériques
#define FINGERPRINT_RX 16
#define FINGERPRINT_TX 17
const char* ssid = "Dailypill-Dashboard";
const char* password = "12345678";
const int buzzerPin = 20;
const int irSensorPin = 22;
const int irLedPin = 21;
const int servoPins[] = {25, 26, 27, 14}; // GPIO25, GPIO26, GPIO27, GPIO14

// Objets globaux
HardwareSerial fingerprintSerial(2);
Adafruit_Fingerprint finger(&fingerprintSerial);
Servo servo1, servo2, servo3, servo4;
SSD1306Wire display(0x3C, SDA, SCL);
AsyncWebServer server(80);
AlarmManager alarmManager;

// === Structures ===
struct Alarm {
    String day;
    String time;
    int stage;
};

// Variables globales
bool alarmActive = false;
int selectedServo = 0;
String alarmTime = "00:00";
std::vector<Alarm> alarms;

// === Fonctions utilitaires ===
void activateBuzzer();
void deactivateBuzzer();
void sendIrSignal();
bool isCupPresent();
void setupDisplay();
void setupWebServer();
void setupFingerprintSensor();
bool verifyFingerprint();
void checkAlarms();

// === Configuration initiale ===
void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialisation des servomoteurs
    servo1.attach(servoPins[0]);
    servo2.attach(servoPins[1]);
    servo3.attach(servoPins[2]);
    servo4.attach(servoPins[3]);

    // Configuration des broches
    pinMode(buzzerPin, OUTPUT);
    deactivateBuzzer();
    pinMode(irSensorPin, INPUT);
    pinMode(irLedPin, OUTPUT);
    digitalWrite(irLedPin, LOW);

    // Initialisation des composants
    setupDisplay();
    if (!LittleFS.begin()) {
        Serial.println("Erreur : Impossible de monter LittleFS !");
        while (1); // Bloquer le programme
    }
    WiFi.softAP(ssid, password);
    setupWebServer();
    setupFingerprintSensor();

    // Ajouter une alarme par défaut
    alarmManager.addAlarm("Test Alarm", 12, 0);

    // Afficher un message de démarrage
    display.clear();
    display.drawString(0, 0, "Systeme pret !");
    display.drawString(0, 10, "Placez votre doigt");
    display.drawString(0, 20, "ou detectez une tasse.");
    display.display();
}

// === Boucle principale ===
void loop() {
    static unsigned long lastAction = 0;
    static int state = 0;

    // Changer d'état toutes les 2 secondes
    if (millis() - lastAction > 2000) {
        state = (state + 1) % 3;
        lastAction = millis();
    }

    switch (state) {
        case 0: // Vérification des empreintes digitales
            if (!alarmActive) {
                activateBuzzer();
                alarmActive = true;
            }
            if (verifyFingerprint()) {
                deactivateBuzzer();
                alarmActive = false;
            }
            break;

        case 1: // Vérification de la présence d'une tasse
            if (isCupPresent()) {
                sendIrSignal();
            }
            break;

        case 2: // Vérification des conditions
            if (alarmActive && isCupPresent() && verifyFingerprint()) {
                Servo* servos[] = {&servo1, &servo2, &servo3, &servo4};
                if (selectedServo >= 0 && selectedServo < 4) {
                    Servo* servo = servos[selectedServo];
                    int initialPosition = servo->read();
                    servo->write(90);
                    delay(500);
                    servo->write(initialPosition);
                }
            }
            break;

        default:
            Serial.println("État inconnu !");
            break;
    }

    checkAlarms();
    delay(100);
}

// === Fonctions spécifiques ===

// Gestion du buzzer
void activateBuzzer() {
    digitalWrite(buzzerPin, HIGH);
    display.clear();
    display.drawString(0, 0, "Alarme activée !");
    display.display();
}

void deactivateBuzzer() {
    digitalWrite(buzzerPin, LOW);
    display.clear();
    display.drawString(0, 0, "Alarme désactivée !");
    display.display();
}

// Gestion du signal IR
void sendIrSignal() {
    digitalWrite(irLedPin, HIGH);
    delay(100);
    digitalWrite(irLedPin, LOW);
    Serial.println("Signal IR envoyé !");
}

// Vérification de la présence d'une tasse
bool isCupPresent() {
    int sensorValue = digitalRead(irSensorPin);
    return sensorValue == HIGH;
}

// Configuration de l'écran OLED
void setupDisplay() {
    display.init();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "ESP32 Dashboard");
    display.display();
}

// Configuration du serveur web
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

    // Autres routes...
    server.begin();
}

// Configuration du capteur d'empreintes
void setupFingerprintSensor() {
    fingerprintSerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);
    finger.begin(57600);
    if (!finger.verifyPassword()) {
        Serial.println("Erreur : Impossible d'initialiser le capteur d'empreintes !");
        while (1);
    }
}

// Vérification des empreintes digitales
bool verifyFingerprint() {
    int result = finger.getImage();
    if (result != FINGERPRINT_OK) return false;

    result = finger.image2Tz();
    if (result != FINGERPRINT_OK) return false;

    result = finger.fingerFastSearch();
    return result == FINGERPRINT_OK;
}

// Vérification des alarmes
void checkAlarms() {
    time_t now = time(nullptr);
    struct tm* currentTime = localtime(&now);

    char currentDay[10];
    strftime(currentDay, sizeof(currentDay), "%A", currentTime);
    char currentTimeStr[6];
    strftime(currentTimeStr, sizeof(currentTimeStr), "%H:%M", currentTime);

    for (size_t i = 0; i < alarms.size(); i++) {
        if (alarms[i].day == String(currentDay) && alarms[i].time == String(currentTimeStr)) {
            activateBuzzer();
            alarms.erase(alarms.begin() + i);
        }
    }
}
