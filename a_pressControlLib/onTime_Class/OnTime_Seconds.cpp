/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 12.22.44
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
#include <lnSerialRead.h>
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
// #
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


    uint32_t current_epoch_seconds = rtc.getEpoch();
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






// ===============================================================
// =   V E C T O R   - V E C T O R   - V E C T O R   - V E C T O R   -
// ===============================================================

// ######################################################################
// # identia a atSecond(uint8_t sec) ma dedicata alla parte con i vector
// ######################################################################
// bool OnTime_Class::atSecondVectorDedicated(uint8_t second) {
bool OnTime_Class::atSecondFlag(uint8_t second) {
    uint8_t current_second = m_timeinfo.tm_sec;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_second_vector_map.count(second)) {
        m_at_last_second_vector = m_at_last_second_vector_map[second];
    }

    if (current_second == second && current_second != m_at_last_second_vector) {
        m_at_last_second_vector_map[second] = current_second; // aggiorniamo il valore
        return true;
    }
    return false;
}


// ######################################################################
// # possiamo fare uso dei vector.
// # quindi impostiamo tutti i valori che vogliamo controllare
// # e la classe provvede a salvare lo stato in una map
// #
// #   const std::vector<uint8_t> seconds = {0, 1, 15, 30, 45, 120};
// #
// ######################################################################

void OnTime_Class::setAtSecondFlag(const std::vector<uint8_t>& seconds) {
    m_at_seconds_to_monitor = seconds;
    for (uint8_t sec : m_at_seconds_to_monitor) {
        LOG_INFO("sec: %d", sec);
    }

}


bool OnTime_Class::getAtSecondFlag(uint8_t second) {
    // LOG_INFO("req sec: %d", second);
    if (m_at_second_flags.count(second)) {  // se lo troviamo
        return m_at_second_flags[second];   // ritorna lo stato
    }
    return false; // Restituisce false se il secondo non è stato registrato
}



// Implement the new method to update the flags
void OnTime_Class::updateAtSecondFlags() {
    for (uint8_t sec : m_at_seconds_to_monitor) {
        // m_at_second_flags[sec] = atSecond(sec) ? true : false;

        // LOG_SPEC("second: %d %d", sec, m_at_second_flags[sec]);


        if (atSecondFlag(sec)) {
            m_at_second_flags[sec] = true;
            // LOG_INFO("------------second true: %d %d", sec, m_at_second_flags[sec]);
        } else {
            m_at_second_flags[sec] = false;
        }
    }





    // uint8_t cur_second = m_timeinfo.tm_sec;
    // if (cur_second != m_at_last_vector_second) {
    //     m_at_last_vector_second = cur_second; // Aggiorna il flag solo al cambio di secondo
    // }
    // for (uint8_t sec : m_at_seconds_to_monitor) {
    //     if (cur_second == sec) {
    //         m_at_second_flags[sec] = true;
    //     } else {
    //         m_at_second_flags[sec] = false;
    //     }
    //     LOG_SPEC("updating second: %d/%d %d", cur_second, sec, m_at_second_flags[sec]);
    // }

    // waitFoEnter();
}