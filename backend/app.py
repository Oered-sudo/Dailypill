from flask import Flask, render_template, request, jsonify
from datetime import datetime
import json

app = Flask(__name__)

# In-memory storage for alarms
alarms = []

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/alarms', methods=['GET'])
def get_alarms():
    return jsonify(alarms)

@app.route('/add_alarm', methods=['POST'])
def add_alarm():
    data = request.json
    alarms.append(data)
    return jsonify({"status": "success", "alarms": alarms})

@app.route('/alarms', methods=['POST'])
def create_alarm():
    data = request.json
    alarm = {
        'id': len(alarms) + 1,
        'name': data['name'],
        'time': data['time'],
        'interval': data.get('interval', None),
        'date': data.get('date', None)
    }
    alarms.append(alarm)
    return jsonify(alarm), 201

@app.route('/alarms/<int:alarm_id>', methods=['PUT'])
def update_alarm(alarm_id):
    data = request.json
    for alarm in alarms:
        if alarm['id'] == alarm_id:
            alarm['name'] = data.get('name', alarm['name'])
            alarm['time'] = data.get('time', alarm['time'])
            alarm['interval'] = data.get('interval', alarm['interval'])
            alarm['date'] = data.get('date', alarm['date'])
            return jsonify(alarm)
    return jsonify({'error': 'Alarm not found'}), 404

@app.route('/alarms/<int:alarm_id>', methods=['DELETE'])
def delete_alarm(alarm_id):
    global alarms
    alarms = [alarm for alarm in alarms if alarm['id'] != alarm_id]
    return jsonify({'result': 'Alarm deleted'})

if __name__ == '__main__':
    app.run(debug=True)