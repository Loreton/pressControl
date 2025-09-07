/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 08.06.23
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


    // // Controlla e gestisci il cambio dei giorni
    if (onDay()) {
        m_at_last_hour_map.clear(); // Reset della mappa delle ore per riattivare i flag
    }


    // // Controlla e gestisci il cambio delle ore
    if (onHour()) {
        #ifdef ON_TIME_CLASS_MINUTES_VECTOR
            m_at_last_minute_map.clear(); // Reset della mappa dei minuti per riattivare i flag
        #endif
    }


    // // Controlla e gestisci il cambio di minuto prima di tutto
    if (onMinute()) {
        m_at_last_second_map.clear();           // Reset della mappa dei secondi per riattivare i flag
        #ifdef ON_TIME_CLASS_SECONDS_VECTOR
            m_at_last_second_vector_map.clear();    // Reset della mappa dei secondi per riattivare i flag
        #endif
    }

    // Esegui l'aggiornamento dei flag dei secondi e dei minuti
    #ifdef ON_TIME_CLASS_SECONDS_VECTOR
        updateAtSecondFlags();
    #endif

    #ifdef ON_TIME_CLASS_MINUTES_VECTOR
        updateAtMinuteFlags();
    #endif
}

