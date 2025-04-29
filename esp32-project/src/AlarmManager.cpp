#include "AlarmManager.h"
#include <Arduino.h>
#include <vector>
#include <string>
#include <time.h> // Pour utiliser la fonction time()

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
    void addAlarm(const String& name, int hour, int minute, bool active) {
        AlarmData newAlarm = { name, hour, minute, active };
        alarms.push_back(newAlarm);
    }

    void removeAlarm(int index) {
        if (index >= 0 && index < alarms.size()) {
            alarms.erase(alarms.begin() + index);
        }
    }

    void updateAlarm(int index, const String& name, int hour, int minute, bool active) {
        if (index >= 0 && index < alarms.size()) {
            alarms[index].name = name;
            alarms[index].hour = hour;
            alarms[index].minute = minute;
            alarms[index].active = active;
        }
    }

    void checkAlarms() {
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

    void triggerAlarm(const AlarmData& alarm) {
        // Code pour déclencher l'alarme (par exemple, activer un buzzer)
    }

    const std::vector<AlarmData>& getAlarms() const {
        return alarms;
    }
};