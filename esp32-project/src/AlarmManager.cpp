#include "AlarmManager.h"
#include <Arduino.h>
#include <vector>
#include <string>
#include <time.h> // Pour utiliser la fonction time()

// The AlarmData struct is now defined in the AlarmManager.h header file.

// The class definition has been removed as it is already in the header file.

// Implémentation des méthodes de AlarmManager
void AlarmManager::addAlarm(const String& name, int hour, int minute, bool active) {
    AlarmData newAlarm = { name, hour, minute, active };
    alarms.push_back(newAlarm);
}
void AlarmManager::removeAlarm(int index) {
    if (index >= 0 && index < alarms.size()) {
        alarms.erase(alarms.begin() + index);
    }
}
void AlarmManager::updateAlarm(int index, const String& name, int hour, int minute, bool active) {
    if (index >= 0 && index < alarms.size()) {
        alarms[index].name = name;
        alarms[index].hour = hour;
        alarms[index].minute = minute;
        alarms[index].active = active;
    }
}
void AlarmManager::checkAlarms() {
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);
    int currentHour = timeInfo->tm_hour;
    int currentMinute = timeInfo->tm_min;
    for (const auto& alarm : alarms) {
        if (alarm.active && alarm.hour == currentHour && alarm.minute == currentMinute) {
            triggerAlarm(alarm);
        }
    }
}
void AlarmManager::triggerAlarm(const AlarmData& alarm) {
    // Code pour déclencher l'alarme (par exemple, activer un buzzer)
    Serial.println("Alarme déclenchée : " + alarm.name);
}
const std::vector<AlarmData>& AlarmManager::getAlarms() const {
    return alarms;
}
bool AlarmManager::isAlarmActive() const {
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);
    int currentHour = timeInfo->tm_hour;
    int currentMinute = timeInfo->tm_min;

    for (const auto& alarm : alarms) {
        if (alarm.active && alarm.hour == currentHour && alarm.minute == currentMinute) {
            return true; // Une alarme active correspond à l'heure actuelle
        }
    }
    return false; // Aucune alarme active
}
