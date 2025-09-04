/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 12.26.06
*/

#include <Arduino.h>
#include <ESP32Time.h> // ESP32Time.cpp

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#include <lnLogger_Class.h>

#include "OnTime_Class.h"

// ----------------------------------------------------
// Classe OnTime_Class utilizzata nel progetto
// La definisco qui per comodità
// ----------------------------------------------------
OnTime_Class onTime;

// Costruttore della classe.
OnTime_Class::OnTime_Class(void) {};

// ################################################################
// #
// ################################################################
void OnTime_Class::setup() {

}

// ################################################################
// #
// ################################################################
void OnTime_Class::update() {
    m_timeinfo = rtc.getTimeStruct(); // lo facciamo qui una volta sola....


    // if (onDay()) {
    //     // La mappa delle ore  è stata resettata.
    // }


    // if (onHour()) {
        // m_at_last_minute_map.clear(); // Reset della mappa dei secondi per riattivare i flag
    // }


    // // Controlla e gestisci il cambio di minuto prima di tutto
    if (onMinute()) {
        m_at_last_second_map.clear();           // Reset della mappa dei secondi per riattivare i flag
        m_at_last_second_vector_map.clear();    // Reset della mappa dei secondi per riattivare i flag
    }

    // Esegui l'aggiornamento dei flag dei secondi e dei minuti
    updateAtSecondFlags();
    // updateAtMinuteFlags(); // Se hai una funzione simile per i minuti

}

