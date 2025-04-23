// filepath: /workspaces/Dailypill/backend/static/script.js

// Référence au formulaire et à la liste des alarmes
const alarmForm = document.getElementById('alarmForm');
const alarmList = document.getElementById('alarmList');

// Gestionnaire d'événements pour soumettre une nouvelle alarme
alarmForm.addEventListener('submit', async (e) => {
    e.preventDefault();

    // Récupération des données du formulaire
    const name = document.getElementById('name').value;
    const date = document.getElementById('date').value;
    const time = document.getElementById('time').value;

    // Envoi des données au serveur
    const response = await fetch('/add_alarm', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, date, time })
    });

    const data = await response.json();
    updateAlarmList(data.alarms);

    // Réinitialisation du formulaire
    alarmForm.reset();
});

// Fonction pour récupérer les alarmes depuis le serveur
async function fetchAlarms() {
    const response = await fetch('/get_alarms');
    const alarms = await response.json();
    updateAlarmList(alarms);
}

// Fonction pour mettre à jour la liste des alarmes
function updateAlarmList(alarms) {
    alarmList.innerHTML = '';
    alarms.forEach((alarm, index) => {
        const li = document.createElement('li');
        li.innerHTML = `
            <span>${alarm.name} - ${alarm.date} à ${alarm.time}</span>
            <button onclick="deleteAlarm(${index})">Supprimer</button>
        `;
        alarmList.appendChild(li);
    });
}

// Fonction pour supprimer une alarme
async function deleteAlarm(index) {
    const response = await fetch(`/delete_alarm/${index}`, { method: 'DELETE' });
    const data = await response.json();
    updateAlarmList(data.alarms);
}

// Charger les alarmes au démarrage
fetchAlarms();