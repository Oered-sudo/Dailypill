#include <Wire.h>
#include <SSD1306Wire.h> // Remplace Adafruit_SSD1306 par SSD1306Wire

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C // Adresse I2C par défaut pour SSD1306

// Initialisation de l'écran avec SSD1306Wire
SSD1306Wire display(OLED_I2C_ADDRESS, SDA, SCL);

void setupDisplay() {
    display.init();
    display.clear();
    display.setFont(ArialMT_Plain_10); // Définit une police compatible avec SSD1306Wire
    display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void displayAlarms(const String* alarmNames, int alarmCount) {
    display.clear();
    int y = 0; // Position verticale initiale
    
    for (int i = 0; i < alarmCount; i++) {
        display.drawString(0, y, alarmNames[i]);
        y += 10; // Incrémente la position verticale pour chaque ligne
    }
    
    display.display();
}

void updateDisplay(const String* alarmNames, int alarmCount) {
    displayAlarms(alarmNames, alarmCount);
}