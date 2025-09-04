/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 17.54.32
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
#include "OnTime_Class.h"




// ######################################################################
// # true se ci troviamo al nuovo minuto)
// ######################################################################
bool OnTime_Class::onHour() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_hour != m_last_hour) {
        m_last_hour = m_timeinfo.tm_hour;

        // Reset della mappa dei minuti per riattivare i flag
        // altrimenti funziona solo per la prima ora
        m_at_last_minute_map.clear();
        return true;
    }
    return false;
}
