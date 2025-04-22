# ESP32 Alarm Web Application

Ce projet est une application de gestion d'alarmes utilisant un ESP32, un écran I2C 128x64 et une interface web. L'application permet aux utilisateurs de programmer des alarmes à des horaires, intervalles et dates précises, ainsi que de modifier les valeurs telles que l'heure et le nom des alarmes.

## Structure du Projet

Le projet est organisé comme suit :

- **backend/** : Contient le code du serveur backend qui gère les requêtes liées aux alarmes.
  - `app.py` : Point d'entrée de l'application backend.
  - `requirements.txt` : Liste des dépendances nécessaires pour le backend.
  - `README.md` : Documentation pour le backend.

- **frontend/** : Contient le code de l'interface utilisateur.
  - `public/index.html` : Page HTML principale.
  - `src/App.js` : Composant principal de l'application React.
  - `src/components/AlarmForm.js` : Composant pour créer de nouvelles alarmes.
  - `src/components/AlarmList.js` : Composant pour afficher et gérer les alarmes.
  - `src/styles/app.css` : Styles CSS pour l'application.
  - `src/utils/api.js` : Fonctions utilitaires pour interagir avec l'API backend.
  - `package.json` : Configuration pour npm.
  - `README.md` : Documentation pour le frontend.

- **esp32/** : Contient le code pour l'ESP32.
  - `main/main.cpp` : Point d'entrée du programme pour l'ESP32.
  - `main/i2c_display.cpp` : Gestion de l'affichage sur l'écran I2C 128x64.
  - `main/alarm_manager.cpp` : Gestion de la logique des alarmes.
  - `platformio.ini` : Configuration pour PlatformIO.
  - `README.md` : Documentation pour le projet ESP32.

## Installation

1. **Backend** :
   - Naviguez dans le dossier `backend`.
   - Installez les dépendances avec `pip install -r requirements.txt`.
   - Lancez le serveur avec `python app.py`.

2. **Frontend** :
   - Naviguez dans le dossier `frontend`.
   - Installez les dépendances avec `npm install`.
   - Lancez l'application avec `npm start`.

3. **ESP32** :
   - Ouvrez le projet dans PlatformIO.
   - Téléversez le code sur l'ESP32.

## Utilisation

- Accédez à l'interface web via votre navigateur pour programmer et gérer vos alarmes.
- Les alarmes programmées seront affichées sur l'écran I2C 128x64 connecté à l'ESP32.

## Contribution

Les contributions sont les bienvenues ! Veuillez soumettre une demande de tirage pour toute amélioration ou correction.