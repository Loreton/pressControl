/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 19.05.30
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
bool OnTime_Class::onDay() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_yday != m_last_yday) {
        m_last_yday = m_timeinfo.tm_yday;

        // Reset della mappa delle ore per riattivare i flag
        m_at_last_hour_map.clear();
        return true;
    }
    return false;
}
