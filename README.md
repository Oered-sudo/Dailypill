# esp32-alarm-webapp

Ce projet est une application web pour programmer des alarmes sur un ESP32, avec un affichage sur un écran I2C de 128x64 pixels. L'application est divisée en deux parties principales : le backend (Flask) et le code pour l'ESP32.

## Structure du projet

- **backend/** : Contient le code du serveur web qui gère les alarmes et sert l'interface utilisateur.
  - `app.py` : Point d'entrée de l'application backend.
  - `templates/index.html` : Interface utilisateur HTML.
  - `static/styles.css` : Styles CSS pour l'application.
  - `static/script.js` : Script JavaScript pour gérer les alarmes.
  - `requirements.txt` : Liste des dépendances nécessaires pour le backend.

- **esp32/** : Contient le code pour l'ESP32.
  - `main/main.cpp` : Point d'entrée du programme pour l'ESP32.
  - `main/i2c_display.cpp` : Gestion de l'affichage sur l'écran I2C.
  - `main/alarm_manager.cpp` : Gestion de la logique des alarmes.
  - `platformio.ini` : Configuration pour PlatformIO.

## Installation

1. Clonez le dépôt :
   ```
   git clone https://github.com/Oered-sudo/Dailypill
   ```

2. Installez les dépendances du backend :
   ```
   cd backend
   pip install -r requirements.txt
   ```

3. Déployez le code sur l'ESP32 en utilisant PlatformIO :
   ```
   cd esp32
   platformio run --target upload
   ```

## Utilisation

1. Lancez le serveur backend :
   ```
   cd backend
   python app.py
   ```

2. Ouvrez votre navigateur et accédez à l'URL suivante :
   ```
   http://127.0.0.1:5000
   ```

3. Utilisez l'interface web pour programmer et gérer vos alarmes.

## Contribuer

Les contributions sont les bienvenues ! Veuillez soumettre une demande de tirage pour toute amélioration ou correction.