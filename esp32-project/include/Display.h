#pragma once

#include <Arduino.h>


void setupDisplay();
void displayAlarms(const String* alarmNames, int alarmCount);
void updateDisplay(const String* alarmNames, int alarmCount);