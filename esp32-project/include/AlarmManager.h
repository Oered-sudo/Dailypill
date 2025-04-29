#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

// Prevent multiple inclusions of this header file
#pragma once

#include <Arduino.h>
#include <vector>


/**
 * Structure représentant une alarme.
 */
struct Alarm {
    String name;
    int hour;
    int minute;
    bool active; // Indique si l'alarme est active
};

/**
 * Classe pour gérer les alarmes.
 */
class AlarmManager {
public:
    /**
     * Ajoute une nouvelle alarme.
     * @param name Nom de l'alarme.
     * @param hour Heure de l'alarme.
     * @param minute Minute de l'alarme.
     */
    void addAlarm(const String& name, int hour, int minute);

    /**
     * Supprime une alarme par son index.
     * @param index Index de l'alarme à supprimer.
     */
    void removeAlarm(int index);

    /**
     * Met à jour une alarme existante.
     * @param index Index de l'alarme à mettre à jour.
     * @param name Nouveau nom de l'alarme.
     * @param hour Nouvelle heure de l'alarme.
     * @param minute Nouvelle minute de l'alarme.
     */
    void updateAlarm(int index, const String& name, int hour, int minute);

    /**
     * Vérifie les alarmes actives et déclenche celles qui doivent l'être.
     */
    void checkAlarms();

    /**
     * Retourne la liste des alarmes.
     * @return Un vecteur contenant toutes les alarmes.
     */
    const std::vector<Alarm>& getAlarms() const;

private:
    /**
     * Déclenche une alarme.
     * @param alarm L'alarme à déclencher.
     */
    void triggerAlarm(const Alarm& alarm);

    std::vector<Alarm> alarms; // Liste des alarmes
};

#endif // ALARMMANAGER_H