#pragma once

#include <Arduino.h>
#include <vector>

struct AlarmData {
    String name;
    int hour;
    int minute;
    bool active;
};

class AlarmManager {
private:
    std::vector<AlarmData> alarms;

public:
    void addAlarm(const String& name, int hour, int minute, bool active = true);
    void removeAlarm(int index);
    void updateAlarm(int index, const String& name, int hour, int minute, bool active);
    void checkAlarms();
    void triggerAlarm(const AlarmData& alarm);
    const std::vector<AlarmData>& getAlarms() const;

    // Nouvelle méthode pour vérifier si une alarme est active
    bool isAlarmActive() const;
};