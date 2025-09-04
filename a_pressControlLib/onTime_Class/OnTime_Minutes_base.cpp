/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 16.50.54
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
// # Controlla se è iniziato un nuovo minute
// ######################################################################
bool OnTime_Class::onMinute() {
    if (m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;
        return true;
    }
    return false;
}


// ######################################################################
// # modulo non è limitato a 60
// ######################################################################
bool OnTime_Class::onMinuteModulo(uint32_t modulo, bool trueOnFirstRun) {
    bool firstRun = true;
    bool isTime = false;
    if (modulo == 0) {modulo = 60; }

    uint32_t last_epoch_minutes = 0;
    if (m_last_epoch_minutes_map.count(modulo)) { // se esiste la entry
        last_epoch_minutes = m_last_epoch_minutes_map[modulo]; // prelevane il valore
        firstRun = false;
    }


    // uint32_t current_minute = rtc.getEpoch();
    // invece di prendere l'epoch() forse è più rapido questo...
    // uint32_t current_minute = m_timeinfo.tm_hour*60*60 + m_timeinfo.tm_min*60 + m_timeinfo.tm_sec;
    uint32_t current_minute = m_timeinfo.tm_hour*60*60 + m_timeinfo.tm_min;

    if (current_minute != last_epoch_minutes && current_minute % modulo == 0) {
        m_last_epoch_minutes_map[modulo] = current_minute; // Aggiorna la mappa
        isTime = true;
    }

    if (!trueOnFirstRun) {firstRun=false; } // se non è stato richiesto il trueOnFirstRun

    return (isTime || firstRun) ? true : false;
}



// ######################################################################
// # std::map<uint8_t, int8_t> m_at_last_minute_map;
// # true se ci troviamo al minuteo x
// # Questa soluzione funziona solo se lo si chiama una volta perché
// # la volta successiva ritorna false
// #
// # Se chiami atMinute(30), la funzione aggiornerà solo
// #    l'entry m_at_last_minute_map[30], senza influenzare un'eventuale
// #    successiva chiamata a atMinute(45).
// ######################################################################
bool OnTime_Class::atMinute(uint8_t minute) {
    uint8_t current_minute = m_timeinfo.tm_min;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_minute_map.count(minute)) {
        m_at_last_minute = m_at_last_minute_map[minute];
    }

    if (current_minute == minute && current_minute != m_at_last_minute) {
        m_at_last_minute_map[minute] = current_minute; // aggiorniamo il valore
        return true;
    }
    return false;
}


