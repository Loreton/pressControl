/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 16.41.19
*/



#ifdef ON_TIME_CLASS_SECONDS_VECTOR

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "OnTime_Class.h"




// ===============================================================
// =   V E C T O R   - V E C T O R   - V E C T O R   - V E C T O R   -
// ===============================================================

// ######################################################################
// # identia a atSecond(uint8_t sec) ma dedicata alla parte con i vector
// ######################################################################
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
    for (uint8_t _second : m_at_seconds_to_monitor) {
        LOG_INFO("_second: %d", _second);
    }

}


// ######################################################################
// #
// ######################################################################
bool OnTime_Class::getAtSecondFlag(uint8_t second) {
    if (m_at_second_flags.count(second)) {  // se lo troviamo
        return m_at_second_flags[second];   // ritorna lo stato
    }
    return false; // Restituisce false se il secondo non è stato registrato
}



// ######################################################################
// # Implement the new method to update the flags
// # must be called in loop or in update()
// ######################################################################
void OnTime_Class::updateAtSecondFlags() {
    for (uint8_t _second : m_at_seconds_to_monitor) {
        m_at_second_flags[_second] = atSecondFlag(_second) ? true : false;
    }
}

        // LOG_SPEC("second: %d %d", sec, m_at_second_flags[sec]);


        // if (atSecondFlag(sec)) {
        //     m_at_second_flags[sec] = true;
        //     // LOG_INFO("------------second true: %d %d", sec, m_at_second_flags[sec]);
        // } else {
        //     m_at_second_flags[sec] = false;
        // }



#else
    #include "OnTime_Class.h"
    void OnTime_Class::updateAtSecondFlags()  {do {} while (0);}
#endif