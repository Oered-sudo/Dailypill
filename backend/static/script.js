// filepath: /workspaces/Dailypill/backend/static/script.js

// Référence au formulaire et à la liste des alarmes
const alarmForm = document.getElementById('alarmForm');
const alarmList = document.getElementById('alarmList');

// Ajouter une alarme
alarmForm.addEventListener('submit', async (e) => {
    e.preventDefault();

    const name = document.getElementById('name').value;
    const date = document.getElementById('date').value;
    const time = document.getElementById('time').value;
    const recurrence = document.getElementById('recurrence').value;

    const response = await fetch('/alarms', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, date, time, recurrence })
    });

    const alarm = await response.json();
    fetchAlarms(); // Rafraîchir la liste des alarmes
    alarmForm.reset();
});

// Récupérer les alarmes
async function fetchAlarms() {
    const response = await fetch('/alarms');
    const alarms = await response.json();
    updateAlarmList(alarms);
}

// Mettre à jour la liste des alarmes
function updateAlarmList(alarms) {
    alarmList.innerHTML = '';
    alarms.forEach((alarm) => {
        const li = document.createElement('li');
        li.innerHTML = `
            <span>${alarm.name} - ${alarm.date || 'Aucune date'} à ${alarm.time} (${alarm.recurrence || 'Pas de récurrence'})</span>
            <button onclick="deleteAlarm(${alarm.id})">Supprimer</button>
        `;
        alarmList.appendChild(li);
    });
}

// Supprimer une alarme
async function deleteAlarm(alarmId) {
    await fetch(`/alarms/${alarmId}`, { method: 'DELETE' });
    fetchAlarms(); // Rafraîchir la liste des alarmes
}

// Charger les alarmes au démarrage
fetchAlarms();