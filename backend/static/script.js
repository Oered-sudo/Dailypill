// filepath: /workspaces/Dailypill/backend/static/script.js

// Référence au formulaire et à la liste des alarmes
const alarmForm = document.getElementById('alarmForm');
const alarmList = document.getElementById('alarmList');
const recurrenceSelect = document.getElementById('recurrence');
const customIntervalLabel = document.getElementById('customIntervalLabel');
const customIntervalInput = document.getElementById('custom_interval');

// Initialiser le sélecteur d'heure
$(document).ready(function () {
    $('#time').timepicker({
        timeFormat: 'HH:mm',
        interval: 30,
        minTime: '00:00',
        maxTime: '23:59',
        defaultTime: '07:00',
        startTime: '00:00',
        dynamic: false,
        dropdown: true,
        scrollbar: true
    });
});

// Afficher ou masquer l'intervalle personnalisé
recurrenceSelect.addEventListener('change', () => {
    if (recurrenceSelect.value === 'custom') {
        customIntervalLabel.style.display = 'block';
        customIntervalInput.style.display = 'block';
    } else {
        customIntervalLabel.style.display = 'none';
        customIntervalInput.style.display = 'none';
    }
});

// Ajouter une alarme
alarmForm.addEventListener('submit', async (e) => {
    e.preventDefault();

    const name = document.getElementById('name').value;
    const time = document.getElementById('time').value;
    const days = Array.from(document.querySelectorAll('#days input:checked')).map(input => parseInt(input.value));
    const recurrence = recurrenceSelect.value;
    const customInterval = customIntervalInput.value ? parseInt(customIntervalInput.value) : null;

    const response = await fetch('/alarms', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, time, days, recurrence, custom_interval: customInterval })
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
            <span>${alarm.name} - ${alarm.time} (${alarm.recurrence || 'Pas de récurrence'})</span>
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