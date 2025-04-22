# Documentation for the ESP32 Alarm Web Application Frontend

This project is a web application designed to manage alarms on an ESP32 device. The frontend is built using React and communicates with a backend server to handle alarm scheduling, updating, and deletion.

## Project Structure

The frontend directory contains the following files:

- **public/index.html**: The main HTML file that loads the React application.
- **src/App.js**: The main component of the React application that manages global state and renders child components.
- **src/components/AlarmForm.js**: A component that allows users to create new alarms by entering details such as time and alarm name.
- **src/components/AlarmList.js**: A component that displays the list of scheduled alarms, enabling users to view and manage their alarms.
- **src/styles/app.css**: CSS styles for the frontend application, defining the appearance of components.
- **src/utils/api.js**: Utility functions for interacting with the backend API, including requests to retrieve, add, or delete alarms.
- **package.json**: Configuration file for npm, listing dependencies and scripts for the frontend project.

## Getting Started

To run the frontend application, follow these steps:

1. **Install Dependencies**: Navigate to the `frontend` directory and run:
   ```
   npm install
   ```

2. **Start the Development Server**: After installing the dependencies, start the application with:
   ```
   npm start
   ```

3. **Access the Application**: Open your web browser and go to `http://localhost:3000` to access the alarm management interface.

## Features

- Create, update, and delete alarms.
- Display alarms on a 128x64 pixel I2C screen connected to an ESP32.
- Modify alarm details such as time and name through a user-friendly interface.

## Contributing

If you would like to contribute to this project, please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.