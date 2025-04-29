#include "AlarmManager.h"

void AlarmManager::addAlarm(const String& name, int hour, int minute) {
    Alarm newAlarm = { name, hour, minute, true };
    alarms.push_back(newAlarm);
}

void AlarmManager::removeAlarm(int index) {
    if (index >= 0 && index < alarms.size()) {
        alarms.erase(alarms.begin() + index);
    }
}

void AlarmManager::updateAlarm(int index, const String& name, int hour, int minute) {
    if (index >= 0 && index < alarms.size()) {
        alarms[index].name = name;
        alarms[index].hour = hour;
        alarms[index].minute = minute;
    }
}

void AlarmManager::checkAlarms() {
    int currentHour = hour();
    int currentMinute = minute();
    for (const auto& alarm : alarms) {
        if (alarm.active && alarm.hour == currentHour && alarm.minute == currentMinute) {
            triggerAlarm(alarm);
        }
    }
}

void AlarmManager::triggerAlarm(const Alarm& alarm) {
    // Code to trigger the alarm (e.g., sound a buzzer, send a notification)
}

const std::vector<Alarm>& AlarmManager::getAlarms() const {
    return alarms;
}