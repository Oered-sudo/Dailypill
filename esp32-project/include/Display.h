#pragma once

#include <Arduino.h>

#include <vector>

void setupDisplay();
void displayAlarms(const String* alarmNames, int alarmCount);
void updateDisplay(const String* alarmNames, int alarmCount);