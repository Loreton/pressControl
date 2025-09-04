/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 14.19.09
*/
#ifdef xxxxxxxxxxxxxxxxxxxxxx
#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "OnTime_Class.h"




// ######################################################################
// # Controlla se è iniziato un nuovo minuto e resetta i flag dei secondi
// ######################################################################
bool OnTime_Class::onMinute() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;

        // Reset della mappa dei secondi per riattivare i flag
        // m_at_last_second_map.clear();
        return true;
    }
    return false;
}




// ######################################################################
// # true se ci troviamo al secondo x
// ######################################################################
// bool OnTime_Class::atMinute2(uint8_t minute) {
//     m_timeinfo = rtc.getTimeStruct();
//     uint8_t current_minute = m_timeinfo.tm_min;

//     // Controlla se il minuto corrente è quello desiderato e se non è già stato registrato per quest'ora
//     if (current_minute == minute && current_minute != m_at_last_minute) {
//         m_at_last_minute = current_minute; // Registra il minuto
//         return true;
//     }
//     return false;
// }


// ######################################################################
// # Questa soluzione funziona solo se lo si chiama una volta perché
// # la volta successiva ritorna false
// ######################################################################
bool OnTime_Class::atMinute(uint8_t minute) {
    m_timeinfo = rtc.getTimeStruct();
    uint8_t current_minute = m_timeinfo.tm_min;

    if (current_minute != m_at_last_minute) {
        m_at_last_minute = current_minute; // Aggiorna il flag solo al cambio di minuteo

        // Poiché il flag viene aggiornato a ogni minuto, questa condizione si attiverà solo una volta per ogni minuto.
        if (current_minute == minute) {
            return true;
        }
    }
    return false;
}




#endif