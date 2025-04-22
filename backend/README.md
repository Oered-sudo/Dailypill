# Backend Documentation for ESP32 Alarm Web Application

This README file provides instructions on how to set up and run the backend for the ESP32 Alarm Web Application.

## Overview

The backend is built using Python and Flask, serving as the API for managing alarms. It allows users to create, update, and delete alarms, which can then be displayed on an I2C 128x64 pixel screen connected to an ESP32.

## Requirements

To run the backend, you need to have Python installed on your machine. Additionally, you will need the following dependencies:

- Flask
- Flask-CORS (if you are making cross-origin requests)

## Installation

1. Clone the repository:

   ```
   git clone https://github.com/yourusername/esp32-alarm-webapp.git
   cd esp32-alarm-webapp/backend
   ```

2. Create a virtual environment (optional but recommended):

   ```
   python -m venv venv
   source venv/bin/activate  # On Windows use `venv\Scripts\activate`
   ```

3. Install the required packages:

   ```
   pip install -r requirements.txt
   ```

## Running the Application

To start the backend server, run the following command:

```
python app.py
```

By default, the server will run on `http://127.0.0.1:5000`. You can change the host and port in the `app.py` file if needed.

## API Endpoints

The backend provides the following API endpoints:

- `GET /alarms`: Retrieve a list of all alarms.
- `POST /alarms`: Create a new alarm.
- `PUT /alarms/<id>`: Update an existing alarm.
- `DELETE /alarms/<id>`: Delete an alarm.

## Usage

You can interact with the API using tools like Postman or through the frontend application. Ensure that the frontend is configured to point to the correct backend URL.

## Contributing

If you would like to contribute to this project, please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.