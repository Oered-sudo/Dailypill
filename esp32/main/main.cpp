#include <Arduino.h>
#include <Wire.h>
#include "i2c_display.h"
#include "alarm_manager.h"

void setup() {
    Serial.begin(115200);
    Wire.begin(); // Initialiser I2C
    initDisplay(); // Initialiser l'écran I2C
    initAlarmManager(); // Initialiser le gestionnaire d'alarmes
}

void loop() {
    checkAlarms(); // Vérifier les alarmes programmées
    displayAlarms(); // Afficher les alarmes sur l'écran
    delay(1000); // Attendre 1 seconde avant la prochaine vérification
}