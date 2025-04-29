#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <SSD1306Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C

void setupDisplay();
void displayAlarms(const String* alarmNames, int alarmCount);
void updateDisplay(const String* alarmNames, int alarmCount);

#endif // DISPLAY_H