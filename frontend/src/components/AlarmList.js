import React, { useEffect, useState } from 'react';
import { fetchAlarms, deleteAlarm } from '../utils/api';

const AlarmList = () => {
    const [alarms, setAlarms] = useState([]);

    useEffect(() => {
        const getAlarms = async () => {
            const data = await fetchAlarms();
            setAlarms(data);
        };
        getAlarms();
    }, []);

    const handleDelete = async (id) => {
        await deleteAlarm(id);
        setAlarms(alarms.filter(alarm => alarm.id !== id));
    };

    return (
        <div>
            <h2>Liste des Alarmes</h2>
            <ul>
                {alarms.map(alarm => (
                    <li key={alarm.id}>
                        {alarm.name} - {alarm.time}
                        <button onClick={() => handleDelete(alarm.id)}>Supprimer</button>
                    </li>
                ))}
            </ul>
        </div>
    );
};

export default AlarmList;