/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 15.18.18
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
// # true se ci troviamo al nuovo minuto)
// ######################################################################
bool LnTime_Class::onHour() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_sec == 0 && m_timeinfo.tm_min == 0 && m_timeinfo.tm_hour != m_last_hour) {
        m_last_hour = m_timeinfo.tm_hour;
        return true;
    }
    return false;
}

