// filepath: /workspaces/Dailypill/backend/static/script.js
document.getElementById('alarmForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const name = document.getElementById('name').value;
    const time = document.getElementById('time').value;

    const response = await fetch('/add_alarm', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, time })
    });

    const data = await response.json();
    updateAlarmList(data.alarms);
});

async function fetchAlarms() {
    const response = await fetch('/get_alarms');
    const alarms = await response.json();
    updateAlarmList(alarms);
}

function updateAlarmList(alarms) {
    const alarmList = document.getElementById('alarmList');
    alarmList.innerHTML = '';
    alarms.forEach(alarm => {
        const li = document.createElement('li');
        li.textContent = `${alarm.name} - ${alarm.time}`;
        alarmList.appendChild(li);
    });
}

fetchAlarms();