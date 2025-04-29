#pragma once

#include <Arduino.h>
#include <vector>

struct Alarm {
    String name;
    int hour;
    int minute;
};

class AlarmManager {
public:
    void addAlarm(const String& name, int hour, int minute);
    bool isAlarmActive();
    const std::vector<Alarm>& getAlarms() const;

private:
    std::vector<Alarm> alarms;
};