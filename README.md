# Dailypill

Dailypill est un projet Arduino conçu pour gérer un système d'alarme configurable à l'aide d'un écran OLED, de boutons physiques et de fonctionnalités avancées comme la sécurité par empreinte digitale et un mécanisme de verrouillage par électroaimant. Ce projet vise à aider les utilisateurs à gérer leurs médicaments de manière sécurisée et efficace.

## Fonctionnalités principales

1. **Sécurité par empreinte digitale**  
   Chronopil utilise un capteur d'empreintes digitales pour s'assurer que seul l'utilisateur autorisé peut accéder à la boîte à pilules.  
   - Cela garantit que les médicaments ne tombent pas entre de mauvaises mains, comme des enfants ou des personnes non autorisées.

2. **Alarmes programmables avec intervalles**  
   - Vous pouvez programmer des rappels pour prendre vos médicaments à des heures précises.  
   - Les alarmes peuvent être configurées avec un intervalle (par exemple, tous les 2 jours) et une date limite (par exemple, jusqu'au 31 décembre 2025).  
   - Une fois l'alarme déclenchée, le système peut afficher un message sur l'écran OLED et déverrouiller la boîte à pilules.

3. **Affichage intuitif avec interface utilisateur**  
   Un écran OLED intégré affiche des informations importantes comme :  
   - L'heure actuelle.  
   - Le calendrier (date du jour).  
   - La liste des alarmes programmées.  
   - Les messages ou instructions pour l'utilisateur.  
   Une interface utilisateur simple permet de naviguer dans les menus à l'aide de boutons pour :  
   - Programmer des alarmes.  
   - Consulter les alarmes existantes.  
   - Déverrouiller la boîte à pilules.

4. **Mécanisme de verrouillage par électroaimant**  
   - Un électroaimant contrôle l'ouverture et la fermeture de la boîte à pilules.  
   - Une fois que l'utilisateur a pris ses médicaments, le système verrouille automatiquement la boîte pour éviter tout accès non autorisé.

5. **Détection de présence**  
   - Un capteur infrarouge détecte si quelqu'un est à proximité de la boîte.  
   - Cela peut être utilisé pour activer automatiquement l'écran ou pour déclencher une action spécifique.

## Comment ça fonctionne ?

### Étape 1 : Programmation des alarmes  
Les alarmes peuvent être programmées directement dans le code ou via l'interface utilisateur.  
- Chaque alarme est définie par une heure précise (par exemple, 8h30), un intervalle (par exemple, tous les 2 jours) et une date limite (par exemple, jusqu'au 31 décembre 2025).

### Étape 2 : Vérification de l'utilisateur  
- Lorsque l'utilisateur souhaite accéder à la boîte à pilules, il doit placer son doigt sur le capteur d'empreintes digitales.  
- Si l'empreinte est reconnue, la boîte se déverrouille automatiquement.  
- Si l'empreinte n'est pas reconnue, l'accès est refusé.

### Étape 3 : Affichage des informations  
L'écran OLED affiche des informations utiles, comme :  
- Les alarmes actives.  
- L'heure et la date actuelles.  
- Des messages pour guider l'utilisateur (par exemple, "Prenez vos médicaments").

### Étape 4 : Déclenchement des alarmes  
- À l'heure programmée, une alarme se déclenche.  
- Le système peut afficher un message sur l'écran, jouer un son (si un buzzer est ajouté) et déverrouiller la boîte à pilules.

### Étape 5 : Fermeture automatique  
- Une fois que l'utilisateur a pris ses médicaments, le système verrouille automatiquement la boîte à pilules grâce à l'électroaimant.

## Matériel requis

- Carte Arduino (par exemple, FireBeetle ESP32 ou similaire)
- Écran OLED (128x64, basé sur SSD1306)
- Capteur d'empreintes digitales
- Électroaimant pour le mécanisme de verrouillage
- Capteur infrarouge pour la détection de présence
- Boutons poussoirs (4 boutons pour la navigation)
- Résistances pour les boutons (si nécessaire)
- Câblage et alimentation

## Dépendances

Assurez-vous d'installer les bibliothèques suivantes dans l'IDE Arduino avant de téléverser le code :

1. **Adafruit GFX Library** : Pour gérer l'affichage graphique.
2. **Adafruit SSD1306** : Pour contrôler l'écran OLED.
3. **RTClib** : Pour la gestion de l'horloge temps réel (RTC).
4. **MenuSystem** : Pour la gestion du menu interactif.
5. **Adafruit Fingerprint Sensor Library** : Pour le capteur d'empreintes digitales.

### Installation des bibliothèques

Vous pouvez installer ces bibliothèques via le gestionnaire de bibliothèques de l'IDE Arduino :
1. Ouvrez l'IDE Arduino.
2. Allez dans **Croquis > Inclure une bibliothèque > Gérer les bibliothèques**.
3. Recherchez et installez les bibliothèques mentionnées ci-dessus.

## Schéma de câblage

1. Connectez l'écran OLED à la carte Arduino via I2C (SDA et SCL).
2. Connectez les boutons aux broches définies dans le code :
   - `BUTTON_NEXT` : Broche 22
   - `BUTTON_INC` : Broche 3
   - `BUTTON_SET` : Broche 4
   - `BUTTON_CANCEL` : Broche 5
3. Connectez le capteur d'empreintes digitales et l'électroaimant selon leurs spécifications.
4. Assurez-vous que les boutons sont configurés avec des résistances pull-up ou utilisez `INPUT_PULLUP` dans le code.

## Utilisation

1. Téléversez le code sur votre carte Arduino.
2. Connectez l'écran OLED, les boutons, le capteur d'empreintes digitales et l'électroaimant selon le schéma.
3. Naviguez dans le menu à l'aide des boutons pour configurer les alarmes et accéder à la boîte à pilules.

## Auteurs

- **Votre Nom** : Développeur principal.

## Licence

Ce projet est sous licence MIT. Vous êtes libre de l'utiliser, de le modifier et de le distribuer.

---