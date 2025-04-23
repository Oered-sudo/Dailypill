// filepath: /workspaces/Dailypill/backend/static/script.js

document.addEventListener('DOMContentLoaded', function () {
    // Initialiser le sélecteur d'heure
    const timepickers = document.querySelectorAll('.timepicker');
    M.Timepicker.init(timepickers, {
        defaultTime: '07:00', // Heure par défaut
        twelveHour: false, // Format 24 heures
        autoClose: true // Fermer automatiquement après la sélection
    });

    // Gestion de la sélection des jours
    const dayButtons = document.querySelectorAll('.day-btn');
    dayButtons.forEach(button => {
        button.addEventListener('click', () => {
            button.classList.toggle('selected');
        });
    });

    // Afficher ou masquer l'intervalle personnalisé
    const recurrenceSelect = document.getElementById('recurrence');
    const customIntervalContainer = document.getElementById('customIntervalContainer');
    recurrenceSelect.addEventListener('change', () => {
        if (recurrenceSelect.value === 'custom') {
            customIntervalContainer.style.display = 'block';
        } else {
            customIntervalContainer.style.display = 'none';
        }
    });
});

// Référence au formulaire et à la liste des alarmes
const alarmForm = document.getElementById('alarmForm');
const alarmList = document.getElementById('alarmList');

// Ajouter une alarme
alarmForm.addEventListener('submit', async (e) => {
    e.preventDefault();

    const name = document.getElementById('name').value;
    const time = document.getElementById('time').value; // Récupérer l'heure sélectionnée
    const days = Array.from(document.querySelectorAll('.day-btn.selected')).map(btn => parseInt(btn.dataset.value));
    const recurrence = document.getElementById('recurrence').value;
    const customInterval = document.getElementById('custom_interval').value || null;

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
        document.querySelectorAll('.day-btn').forEach(btn => btn.classList.remove('selected'));
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
            <button onclick="deleteAlarm(${alarm.id})" class="btn red">Supprimer</button>
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