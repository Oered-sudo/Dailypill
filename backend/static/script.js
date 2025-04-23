// filepath: /workspaces/Dailypill/backend/static/script.js

// Référence au formulaire et à la liste des alarmes
const alarmForm = document.getElementById('alarmForm');
const alarmList = document.getElementById('alarmList');
const recurrenceSelect = document.getElementById('recurrence');
const customIntervalLabel = document.getElementById('customIntervalLabel');
const customIntervalInput = document.getElementById('custom_interval');

// Vérifiez si l'élément timePicker existe avant d'initialiser la roue
let timePicker;
if (document.getElementById('timePicker')) {
    timePicker = new WheelTimePicker({
        element: document.getElementById('timePicker'),
        format: 'HH:mm', // Format 24 heures
        interval: 1, // Intervalle de 1 minute
        defaultTime: '07:00', // Heure par défaut
    });
}

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
    const time = timePicker ? timePicker.getTime() : document.getElementById('time').value; // Récupérer l'heure sélectionnée
    const days = Array.from(document.querySelectorAll('#days input:checked')).map(input => parseInt(input.value));
    const recurrence = recurrenceSelect.value;
    const customInterval = customIntervalInput.value ? parseInt(customIntervalInput.value) : null;

    try {
        const response = await fetch('/alarms', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name, time, days, recurrence, custom_interval: customInterval })
        });

        if (!response.ok) {
            throw new Error('Erreur lors de l\'ajout de l\'alarme');
        }

        const alarm = await response.json();
        fetchAlarms(); // Rafraîchir la liste des alarmes
        alarmForm.reset();
    } catch (error) {
        console.error('Erreur :', error);
        alert('Une erreur est survenue lors de l\'ajout de l\'alarme.');
    }
});

// Récupérer les alarmes
async function fetchAlarms() {
    try {
        const response = await fetch('/alarms');
        if (!response.ok) {
            throw new Error('Erreur lors de la récupération des alarmes');
        }

        const alarms = await response.json();
        updateAlarmList(alarms);
    } catch (error) {
        console.error('Erreur :', error);
        alert('Une erreur est survenue lors de la récupération des alarmes.');
    }
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
    try {
        const response = await fetch(`/alarms/${alarmId}`, { method: 'DELETE' });
        if (!response.ok) {
            throw new Error('Erreur lors de la suppression de l\'alarme');
        }

        fetchAlarms(); // Rafraîchir la liste des alarmes
    } catch (error) {
        console.error('Erreur :', error);
        alert('Une erreur est survenue lors de la suppression de l\'alarme.');
    }
}

// Charger les alarmes au démarrage
fetchAlarms();