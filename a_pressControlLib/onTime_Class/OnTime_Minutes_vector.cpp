/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 16.40.59
*/



#ifdef ON_TIME_CLASS_MINUTES_VECTOR

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
// # identia a atMinute(uint8_t sec) ma dedicata alla parte con i vector
// ######################################################################
bool OnTime_Class::atMinuteFlag(uint8_t minute) {
    uint8_t current_minute = m_timeinfo.tm_min;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_minute_vector_map.count(minute)) {
        m_at_last_minute_vector = m_at_last_minute_vector_map[minute];
    }

    if (current_minute == minute && current_minute != m_at_last_minute_vector) {
        m_at_last_minute_vector_map[minute] = current_minute; // aggiorniamo il valore
        return true;
    }
    return false;
}


// ######################################################################
// # possiamo fare uso dei vector.
// # quindi impostiamo tutti i valori che vogliamo controllare
// # e la classe provvede a salvare lo stato in una map
// #
// #   const std::vector<uint8_t> minutes = {0, 1, 15, 30, 45, 120};
// #
// ######################################################################
void OnTime_Class::setAtMinuteFlag(const std::vector<uint8_t>& minutes) {
    m_at_minutes_to_monitor = minutes;
    for (uint8_t _minute : m_at_minutes_to_monitor) {
        LOG_INFO("_minute: %d", _minute);
    }

}


// ######################################################################
// #
// ######################################################################
bool OnTime_Class::getAtMinuteFlag(uint8_t minute) {
    if (m_at_minute_flags.count(minute)) {  // se lo troviamo
        return m_at_minute_flags[minute];   // ritorna lo stato
    }
    return false; // Restituisce false se il minuteo non è stato registrato
}



// ######################################################################
// # Implement the new method to update the flags
// # must be called in loop or in update()
// ######################################################################
void OnTime_Class::updateAtMinuteFlags() {
    for (uint8_t _minute : m_at_minutes_to_monitor) {
        m_at_minute_flags[_minute] = atMinuteFlag(_minute) ? true : false;
    }
}



#else
    #include "OnTime_Class.h"
    void OnTime_Class::updateAtMinuteFlags()  {do {} while (0);}
#endif