/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 16.59.07
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "OnTime_Class.h"



// ######################################################################
// # Controlla se è iniziato un nuovo secondo
// ######################################################################
bool OnTime_Class::onSecond() {
    if (m_timeinfo.tm_sec != m_last_second) {
        m_last_second = m_timeinfo.tm_sec;
        return true;
    }
    return false;
}


// ######################################################################
// # modulo non è limitato a 60
// ######################################################################
bool OnTime_Class::onSecondModulo(uint32_t modulo, bool trueOnFirstRun) {
    bool firstRun = true;
    bool isTime = false;
    if (modulo == 0) {modulo = 60; }

    uint32_t last_epoch_seconds = 0;
    if (m_last_epoch_seconds_map.count(modulo)) { // se esiste la entry
        last_epoch_seconds = m_last_epoch_seconds_map[modulo]; // prelevane il valore
        firstRun = false;
    }


    // uint32_t current_epoch_seconds = rtc.getEpoch();
    // invece di prendere l'epoch() forse è più rapido questo...
    uint32_t current_epoch_seconds = m_timeinfo.tm_hour*60*60 + m_timeinfo.tm_min*60 + m_timeinfo.tm_sec;

    if (current_epoch_seconds != last_epoch_seconds && current_epoch_seconds % modulo == 0) {
        m_last_epoch_seconds_map[modulo] = current_epoch_seconds; // Aggiorna la mappa
        isTime = true;
    }

    if (!trueOnFirstRun) {firstRun=false; } // se non è stato richiesto il trueOnFirstRun

    return (isTime || firstRun) ? true : false;
}



// ######################################################################
// # std::map<uint8_t, int8_t> m_at_last_second_map;
// # true se ci troviamo al secondo x
// # Questa soluzione funziona solo se lo si chiama una volta perché
// # la volta successiva ritorna false
// #
// # Se chiami atSecond(30), la funzione aggiornerà solo
// #    l'entry m_at_last_second_map[30], senza influenzare un'eventuale
// #    successiva chiamata a atSecond(45).
// ######################################################################
bool OnTime_Class::atSecond(uint8_t second) {
    uint8_t current_second = m_timeinfo.tm_sec;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_second_map.count(second)) {
        m_at_last_second = m_at_last_second_map[second];
    }

    if (current_second == second && current_second != m_at_last_second) {
        m_at_last_second_map[second] = current_second; // aggiorniamo il valore
        return true;
    }
    return false;
}


