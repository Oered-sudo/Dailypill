# esp32-alarm-webapp

Ce projet est une application web pour programmer des alarmes sur un ESP32, avec un affichage sur un écran I2C de 128x64 pixels. L'application est divisée en deux parties principales : le backend et le frontend.

## Structure du projet

- **backend/** : Contient le code du serveur web qui gère les alarmes.
  - `app.py` : Point d'entrée de l'application backend.
  - `requirements.txt` : Liste des dépendances nécessaires pour le backend.
  - `README.md` : Documentation pour le backend.

- **frontend/** : Contient le code de l'application web.
  - `public/index.html` : Page HTML principale.
  - `src/App.js` : Composant principal de l'application React.
  - `src/components/AlarmForm.js` : Composant pour créer de nouvelles alarmes.
  - `src/components/AlarmList.js` : Composant pour afficher la liste des alarmes.
  - `src/styles/app.css` : Styles CSS pour l'application.
  - `src/utils/api.js` : Fonctions utilitaires pour interagir avec l'API backend.
  - `package.json` : Configuration pour npm.
  - `README.md` : Documentation pour le frontend.

- **esp32/** : Contient le code pour l'ESP32.
  - `main/main.cpp` : Point d'entrée du programme pour l'ESP32.
  - `main/i2c_display.cpp` : Gestion de l'affichage sur l'écran I2C.
  - `main/alarm_manager.cpp` : Gestion de la logique des alarmes.
  - `platformio.ini` : Configuration pour PlatformIO.
  - `README.md` : Documentation pour le projet ESP32.

## Installation

1. Clonez le dépôt :
   ```
   git clone <URL_DU_DEPOT>
   ```

2. Installez les dépendances du backend :
   ```
   cd backend
   pip install -r requirements.txt
   ```

3. Installez les dépendances du frontend :
   ```
   cd frontend
   npm install
   ```

4. Déployez le code sur l'ESP32 en utilisant PlatformIO.

## Utilisation

- Lancez le serveur backend :
  ```
  python app.py
  ```

- Ouvrez l'application frontend dans votre navigateur pour programmer et gérer vos alarmes.

## Contribuer

Les contributions sont les bienvenues ! Veuillez soumettre une demande de tirage pour toute amélioration ou correction.