# ESP32 Alarm Web Application

Ce projet est une application de gestion d'alarmes utilisant un ESP32, un écran I2C 128x64 et une interface web. L'application permet aux utilisateurs de programmer des alarmes à des horaires, intervalles et dates précises, ainsi que de modifier les valeurs telles que l'heure et le nom des alarmes.

## Structure du Projet

Le projet est organisé comme suit :

- **backend/** : Contient le code du serveur backend qui gère les requêtes liées aux alarmes et sert l'interface utilisateur.
  - `app.py` : Point d'entrée de l'application backend.
  - `templates/index.html` : Interface utilisateur HTML.
  - `static/styles.css` : Styles CSS pour l'application.
  - `static/script.js` : Script JavaScript pour gérer les alarmes.
  - `requirements.txt` : Liste des dépendances nécessaires pour le backend.

- **esp32/** : Contient le code pour l'ESP32.
  - `main/main.cpp` : Point d'entrée du programme pour l'ESP32.
  - `main/i2c_display.cpp` : Gestion de l'affichage sur l'écran I2C 128x64.
  - `main/alarm_manager.cpp` : Gestion de la logique des alarmes.
  - `platformio.ini` : Configuration pour PlatformIO.

## Installation

1. **Backend** :
   - Naviguez dans le dossier `backend`.
   - Installez les dépendances avec :
     ```bash
     pip install -r requirements.txt
     ```
   - Lancez le serveur avec :
     ```bash
     python app.py
     ```

2. **ESP32** :
   - Installez PlatformIO si ce n'est pas déjà fait :
     ```bash
     pip install platformio
     ```
   - Naviguez dans le dossier `esp32`.
   - Téléversez le code sur l'ESP32 avec :
     ```bash
     platformio run --target upload
     ```

## Utilisation

1. Lancez le serveur backend :
   ```bash
   cd backend
   python app.py
   ```

2. Accédez à l'interface web via votre navigateur à l'adresse suivante :
   ```
   http://127.0.0.1:5000
   ```

3. Utilisez l'interface pour programmer et gérer vos alarmes. Les alarmes programmées seront affichées sur l'écran I2C 128x64 connecté à l'ESP32.

## Contribution

Les contributions sont les bienvenues ! Veuillez soumettre une demande de tirage pour toute amélioration ou correction.