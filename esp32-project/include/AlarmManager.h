#pragma once

#include <Arduino.h>
#include <vector>
#include <string>

struct AlarmData {
    std::string name;
    int hour;
    int minute;
    bool active;
};

class AlarmManager {
private:
    std::vector<AlarmData> alarms;

public:
    void addAlarm(const std::string& name, int hour, int minute, bool active = true);
    void removeAlarm(int index);
    void updateAlarm(int index, const std::string& name, int hour, int minute, bool active);
    void checkAlarms();
    void triggerAlarm(const AlarmData& alarm);
    const std::vector<AlarmData>& getAlarms() const;
    void toggleAlarm(int index, bool active);
};