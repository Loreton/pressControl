/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 16.58.55
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp
#include "esp_sntp.h"
#include <WiFi.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
// #include <lnTimer_Class.h>
#include "LnTime_Class.h"



// ######################################################################
// # Controlla se è iniziato un nuovo minuto e resetta i flag dei secondi
// ######################################################################
bool LnTime_Class::onMinute() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;

        // Reset della mappa dei secondi per riattivare i flag
        m_at_last_second_map.clear();
        return true;
    }
    return false;
}



bool LnTime_Class::atMinute(int8_t minute) {
    m_timeinfo = rtc.getTimeStruct();

    // Controlla se il minuto corrente è quello desiderato e se non è già stato registrato per quest'ora
    if (m_timeinfo.tm_min == minute && m_at_last_minute_map.find(minute) == m_at_last_minute_map.end()) {
        m_at_last_minute_map[minute] = m_timeinfo.tm_min; // Registra il minuto
        return true;
    }
    return false;
}


// ######################################################################
// # true se ci troviamo al nuovo minuto)
// ######################################################################
// bool LnTime_Class::onMinute() {
//     m_timeinfo = rtc.getTimeStruct();
//     if (m_timeinfo.tm_sec == 0 && m_timeinfo.tm_min != m_last_minute) {
//         m_last_minute = m_timeinfo.tm_min;
//         return true;
//     }
//     return false;
// }

// ######################################################################
// # true se ci troviamo al minuto (x)
// ######################################################################
bool LnTime_Class::atMinute(int8_t minute) {
    m_timeinfo = rtc.getTimeStruct();

    // Trova l'ultimo minuto registrato per questo valore "minute".
    int8_t last_at_minute = -1;
    if (m_at_last_minute_map.count(minute)) {
        last_at_minute = m_at_last_minute_map[minute];
    }

    if (m_timeinfo.tm_sec == 0 && m_timeinfo.tm_min == minute && m_timeinfo.tm_min != last_at_minute) {
        m_at_last_minute_map[minute] = m_timeinfo.tm_min;
        return true;
    }
    return false;
}




// ######################################################################
// # true se ci troviamo al minuto mod (x)
// # lavorando sull'epoch time possiamo superare anche il 60
// ###############################################################
bool LnTime_Class::onMinuteModulo(uint32_t modulo, bool trueOnFirstRun) {
    bool firstRun = true;
    bool isTime = false;
    if (modulo == 0) {modulo = 60; }

    uint32_t current_epoch_minutes = rtc.getEpoch() / 60;

    uint32_t last_epoch_minutes = 0;
    if (m_last_epoch_minutes_map.count(modulo)) { // se esiste la entry
        last_epoch_minutes = m_last_epoch_minutes_map[modulo]; // prelevane il valore
        firstRun = false;
    }


    if (current_epoch_minutes != last_epoch_minutes && current_epoch_minutes % modulo == 0) {
        m_last_epoch_minutes_map[modulo] = current_epoch_minutes; // Aggiorna la mappa
        isTime = true;
    }

    if (!trueOnFirstRun) {firstRun=false; } // se non è stato richiesto il trueOnFirstRun

    return (isTime || firstRun) ? true : false;
}



void LnTime_Class::setAtMinuteFlag(const std::vector<uint8_t>& minutes) {
    m_at_minutes_to_monitor = minutes;
}

bool LnTime_Class::getAtMinuteFlag(uint8_t minute) {
    if (m_at_minute_flags.count(minute)) {
        return m_at_minute_flags[minute];
    }
    return false; // Restituisce false se il minuto non è stato registrato
}

// Implement the new method to update the flags
void LnTime_Class::updateAtMinuteFlags() {
    for (uint8_t m : m_at_minutes_to_monitor) {
        if (atMinute(m)) {
            m_at_minute_flags[m] = true;
        } else {
            m_at_minute_flags[m] = false;
        }
    }
}

