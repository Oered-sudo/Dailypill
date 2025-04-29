#include "Display.h"

SSD1306Wire display(OLED_I2C_ADDRESS, SDA, SCL);

void setupDisplay() {
    display.init();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void displayAlarms(const String* alarmNames, int alarmCount) {
    display.clear();
    int y = 0;
    for (int i = 0; i < alarmCount; i++) {
        display.drawString(0, y, alarmNames[i]);
        y += 10;
    }
    display.display();
}

void updateDisplay(const String* alarmNames, int alarmCount) {
    displayAlarms(alarmNames, alarmCount);
}