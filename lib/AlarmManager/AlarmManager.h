#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Arduino.h>
#include <vector>
#include <string>

struct Alarm {
    String name;
    int hour;
    int minute;
    bool active;
};

class AlarmManager {
private:
    std::vector<Alarm> alarms;

public:
    void addAlarm(const String& name, int hour, int minute);
    void removeAlarm(int index);
    void updateAlarm(int index, const String& name, int hour, int minute);
    void checkAlarms();
    void triggerAlarm(const Alarm& alarm);
    const std::vector<Alarm>& getAlarms() const;
};

#endif // ALARM_MANAGER_H