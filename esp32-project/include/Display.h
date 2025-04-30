#pragma once

#include <Arduino.h>
#include <vector>
#include <SSD1306Wire.h>

#ifndef DISPLAY_H
#define DISPLAY_H

// Déclaration externe de l'objet display
extern SSD1306Wire display;

// Déclaration de la fonction setupDisplay
void setupDisplay();

void displayAlarms(const String* alarmNames, int alarmCount);
void updateDisplay(const String* alarmNames, int alarmCount);

#endif // DISPLAY_H