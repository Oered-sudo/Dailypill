import React, { useState } from 'react';

const AlarmForm = ({ onAddAlarm }) => {
    const [alarmName, setAlarmName] = useState('');
    const [alarmTime, setAlarmTime] = useState('');
    const [alarmInterval, setAlarmInterval] = useState('');
    const [alarmDate, setAlarmDate] = useState('');

    const handleSubmit = (e) => {
        e.preventDefault();
        if (!alarmName || !alarmTime) return;

        const newAlarm = {
            name: alarmName,
            time: alarmTime,
            interval: alarmInterval,
            date: alarmDate,
        };

        onAddAlarm(newAlarm);
        setAlarmName('');
        setAlarmTime('');
        setAlarmInterval('');
        setAlarmDate('');
    };

    return (
        <form onSubmit={handleSubmit}>
            <div>
                <label>Nom de l'alarme:</label>
                <input
                    type="text"
                    value={alarmName}
                    onChange={(e) => setAlarmName(e.target.value)}
                    required
                />
            </div>
            <div>
                <label>Heure de l'alarme:</label>
                <input
                    type="time"
                    value={alarmTime}
                    onChange={(e) => setAlarmTime(e.target.value)}
                    required
                />
            </div>
            <div>
                <label>Intervalle (en minutes):</label>
                <input
                    type="number"
                    value={alarmInterval}
                    onChange={(e) => setAlarmInterval(e.target.value)}
                />
            </div>
            <div>
                <label>Date de l'alarme:</label>
                <input
                    type="date"
                    value={alarmDate}
                    onChange={(e) => setAlarmDate(e.target.value)}
                />
            </div>
            <button type="submit">Ajouter l'alarme</button>
        </form>
    );
};

export default AlarmForm;