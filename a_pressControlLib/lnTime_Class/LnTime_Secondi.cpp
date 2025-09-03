/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 15.28.18
*/



// LnTime_Secondi.cpp

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








// Allinea l'esecuzione all'inizio del minuto
// void LnTime_Class::alignToMinute() {
//     LOG_DEBUG("waiting for minute o'clock");
//     m_timeinfo = rtc.getTimeStruct();
//     while (m_timeinfo.tm_sec != 0) {
//         delay(50); // Piccolo ritardo per evitare busy-waiting eccessivo
//         m_timeinfo = rtc.getTimeStruct();
//     }
// }

// ######################################################################
// # Controlla se è iniziato un nuovo secondo
// ######################################################################
bool LnTime_Class::onSecond() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_sec != m_last_second) {
        m_last_second = m_timeinfo.tm_sec;
        return true;
    }
    return false;
}



// ######################################################################
// # true se ci troviamo al secondo x
// # lavorando sull'epoch time possiamo superare anche il 60
// ######################################################################
bool LnTime_Class::atSecond(uint8_t second) {
    m_timeinfo = rtc.getTimeStruct();

    // Controlla se il secondo corrente è quello desiderato e se non è già stato registrato per questo minuto
    if (m_timeinfo.tm_sec == second && m_at_last_second_map.find(second) == m_at_last_second_map.end()) {
        m_at_last_second_map[second] = m_timeinfo.tm_sec; // Registra il secondo
        return true;
    }
    return false;
}

// bool LnTime_Class::atSecond(uint8_t second) {
//     m_timeinfo = rtc.getTimeStruct();

//     // Trova l'ultimo secondo registrato per questo valore "second".
//     // Se non esiste, il valore di default è -1 (o un valore che non si ripete mai) per la prima esecuzione.
//     int8_t last_at_second = -1;
//     if (m_at_last_second_map.count(second)) {
//         last_at_second = m_at_last_second_map[second];
//     }

//     if (m_timeinfo.tm_sec == second && m_timeinfo.tm_sec != last_at_second) {
//         m_at_last_second_map[second] = m_timeinfo.tm_sec;
//         return true;
//     }
//     return false;
// }





// ######################################################################
// # true se ci troviamo al minuto mod (x)
// # lavorando sull'epoch time possiamo superare anche il 60
// ###############################################################
bool LnTime_Class::onSecondModulo(uint32_t modulo, bool trueOnFirstRun) {
    bool firstRun = true;
    bool isTime = false;
    if (modulo == 0) {modulo = 60; }


    uint32_t last_epoch_seconds = 0;
    if (m_last_epoch_seconds_map.count(modulo)) { // se esiste la entry
        last_epoch_seconds = m_last_epoch_seconds_map[modulo]; // prelevane il valore
        firstRun = false;
    }

    uint32_t current_epoch_seconds = rtc.getEpoch();

    if (current_epoch_seconds != last_epoch_seconds && current_epoch_seconds % modulo == 0) {
        m_last_epoch_seconds_map[modulo] = current_epoch_seconds; // Aggiorna la mappa
        isTime = true;
    }

    if (!trueOnFirstRun) {firstRun=false; } // se non è stato richiesto il trueOnFirstRun

    return (isTime || firstRun) ? true : false;
}











void LnTime_Class::setAtSecondFlag(const std::vector<uint8_t>& seconds) {
    m_at_seconds_to_monitor = seconds;
}


bool LnTime_Class::getAtSecondFlag(uint8_t second) {
    if (m_at_second_flags.count(second)) {
        return m_at_second_flags[second];
    }
    return false; // Restituisce false se il secondo non è stato registrato
}



// Implement the new method to update the flags
void LnTime_Class::updateAtSecondFlags() {
    for (uint8_t sec : m_at_seconds_to_monitor) {
        // m_at_second_flags[sec] = atSecond(sec) ? true : false;

        // LOG_SPEC("second: %d %d", sec, m_at_second_flags[sec]);


        if (atSecond(sec)) {
            m_at_second_flags[sec] = true;
            LOG_WARN("------------second true: %d %d", sec, m_at_second_flags[sec]);
        } else {
            m_at_second_flags[sec] = false;
        }
    }
}