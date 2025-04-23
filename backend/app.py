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
            # Vérifie si l'alarme est récurrente ou ponctuelle
            alarm_time = datetime.strptime(alarm['time'], "%H:%M").time()
            alarm_date = datetime.strptime(alarm['date'], "%Y-%m-%d").date() if alarm['date'] else None

            # Déclenchement des alarmes ponctuelles
            if alarm_date and alarm_date == now.date() and alarm_time <= now.time():
                print(f"Alarm triggered: {alarm['name']}")
                alarms.remove(alarm)

            # Déclenchement des alarmes récurrentes
            if alarm['recurrence'] == "daily" and alarm_time <= now.time():
                print(f"Recurring alarm triggered: {alarm['name']}")
                alarm['date'] = (now + timedelta(days=1)).strftime("%Y-%m-%d")

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
        'date': data.get('date', None),
        'recurrence': data.get('recurrence', None)  # New field for recurrence
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
            alarm['date'] = data.get('date', alarm['date'])
            alarm['recurrence'] = data.get('recurrence', alarm['recurrence'])
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