#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
    display.begin(SSD1306_I2C_ADDRESS, OLED_RESET);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void displayAlarms(const String* alarmNames, int alarmCount) {
    display.clearDisplay();
    display.setCursor(0, 0);
    
    for (int i = 0; i < alarmCount; i++) {
        display.println(alarmNames[i]);
    }
    
    display.display();
}

void updateDisplay(const String* alarmNames, int alarmCount) {
    displayAlarms(alarmNames, alarmCount);
}