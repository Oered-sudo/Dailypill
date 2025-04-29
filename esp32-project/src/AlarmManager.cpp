#include "AlarmManager.h"

void AlarmManager::addAlarm(const String& name, int hour, int minute) {
    alarms.push_back({name, hour, minute});
}

bool AlarmManager::isAlarmActive() {
    // Vérifiez si une alarme est active (ajoutez votre logique ici)
    return false;
}

const std::vector<Alarm>& AlarmManager::getAlarms() const {
    return alarms;
}