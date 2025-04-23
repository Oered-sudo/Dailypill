from flask import Flask, render_template, request, jsonify
from datetime import datetime, timedelta
import threading
import time

app = Flask(__name__)

# In-memory storage for alarms
alarms = []

# Function to handle recurring alarms
def alarm_loop():
    while True:
        now = datetime.now()
        for alarm in alarms:
            alarm_time = datetime.strptime(alarm['time'], "%H:%M").time()
            alarm_days = alarm.get('days', [])  # Liste des jours sélectionnés (0 = Lundi, 6 = Dimanche)

            # Vérifie si l'alarme doit être déclenchée aujourd'hui
            if now.weekday() in alarm_days and alarm_time <= now.time():
                print(f"Alarm triggered: {alarm['name']}")

                # Gestion des récurrences personnalisées
                if alarm['recurrence'] == "custom":
                    next_trigger = datetime.strptime(alarm['next_trigger'], "%Y-%m-%d")
                    if now.date() >= next_trigger.date():
                        alarm['next_trigger'] = (next_trigger + timedelta(days=alarm['custom_interval'])).strftime("%Y-%m-%d")
                        print(f"Custom recurring alarm rescheduled: {alarm['name']}")

                # Gestion des alarmes quotidiennes
                elif alarm['recurrence'] == "daily":
                    print(f"Daily alarm triggered: {alarm['name']}")

                # Gestion des alarmes hebdomadaires
                elif alarm['recurrence'] == "weekly":
                    print(f"Weekly alarm triggered: {alarm['name']}")

        time.sleep(60)  # Vérifie les alarmes toutes les minutes

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/alarms', methods=['GET'])
def get_alarms():
    """Retrieve all alarms."""
    return jsonify(alarms)

@app.route('/alarms', methods=['POST'])
def create_alarm():
    """Create a new alarm."""
    data = request.json
    alarm = {
        'id': len(alarms) + 1,
        'name': data['name'],
        'time': data['time'],
        'days': data.get('days', []),  # Liste des jours sélectionnés
        'recurrence': data.get('recurrence', None),  # Type de récurrence
        'custom_interval': data.get('custom_interval', None),  # Intervalle personnalisé (en jours)
        'next_trigger': data.get('next_trigger', None)  # Prochaine date de déclenchement pour les récurrences personnalisées
    }
    alarms.append(alarm)
    return jsonify(alarm), 201

@app.route('/alarms/<int:alarm_id>', methods=['PUT'])
def update_alarm(alarm_id):
    """Update an existing alarm."""
    data = request.json
    for alarm in alarms:
        if alarm['id'] == alarm_id:
            alarm['name'] = data.get('name', alarm['name'])
            alarm['time'] = data.get('time', alarm['time'])
            alarm['days'] = data.get('days', alarm['days'])
            alarm['recurrence'] = data.get('recurrence', alarm['recurrence'])
            alarm['custom_interval'] = data.get('custom_interval', alarm['custom_interval'])
            alarm['next_trigger'] = data.get('next_trigger', alarm['next_trigger'])
            return jsonify(alarm)
    return jsonify({'error': 'Alarm not found'}), 404

@app.route('/alarms/<int:alarm_id>', methods=['DELETE'])
def delete_alarm(alarm_id):
    """Delete an alarm."""
    global alarms
    alarms = [alarm for alarm in alarms if alarm['id'] != alarm_id]
    return jsonify({'result': 'Alarm deleted', 'alarms': alarms})

@app.route('/add_alarm', methods=['POST'])
def add_alarm():
    """Legacy route for adding alarms."""
    data = request.json
    alarms.append(data)
    return jsonify({"status": "success", "alarms": alarms})

if __name__ == '__main__':
    # Start the alarm loop in a separate thread
    threading.Thread(target=alarm_loop, daemon=True).start()
    app.run(debug=True)