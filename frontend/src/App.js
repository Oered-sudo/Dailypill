import React, { useState, useEffect } from 'react';
import './styles/app.css';
import AlarmForm from './components/AlarmForm';
import AlarmList from './components/AlarmList';
import { fetchAlarms } from './utils/api';

function App() {
    const [alarms, setAlarms] = useState([]);

    useEffect(() => {
        const loadAlarms = async () => {
            const fetchedAlarms = await fetchAlarms();
            setAlarms(fetchedAlarms);
        };
        loadAlarms();
    }, []);

    return (
        <div className="App">
            <h1>Alarm Scheduler</h1>
            <AlarmForm setAlarms={setAlarms} />
            <AlarmList alarms={alarms} setAlarms={setAlarms} />
        </div>
    );
}

export default App;