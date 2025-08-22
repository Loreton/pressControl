/*
// updated by ...: Loreto Notarantonio
// Date .........: 22-08-2025 18.14.20
*/

#include <Arduino.h>
#include <ESP32Time.h> // ESP32Time.cpp

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #include <lnLogger_Class.h>



#include "onTime_Class.h"


// Costruttore della classe.
OnTime_Class::OnTime_Class(void) {};



// qui non si fanno controlli particolari
// quindi se il metodo viene chiamato fuori tempo l'evento viene ignorato

// true: se ci troviamo nel secondo richiestso
bool OnTime_Class::atSecond(uint8_t second) {
    m_timeinfo = rtc.getTimeStruct();
    m_curr_second = m_timeinfo.tm_sec;

    if (m_curr_second == second && m_curr_second != m_last_second) { // ogni 5 secondi
        m_last_second = m_curr_second;
        return true;
    }
    return false;
}


// true: se ci troviamo nel minuto richiestso
bool OnTime_Class::atMinute(uint8_t minute) {
    m_timeinfo = rtc.getTimeStruct();
    m_curr_minute = m_timeinfo.tm_min;
    if (m_curr_minute == minute && m_curr_minute != m_last_minute) { // ogni 5 minutei
        m_last_minute = m_curr_minute;
        return true;
    }
    return false;
}

// true: se ci troviamo nel modulo del secondo richiestso (Sec%reqSec)
bool OnTime_Class::onSecondModulo(uint8_t seconds) {

    m_timeinfo = rtc.getTimeStruct();
    m_curr_second = m_timeinfo.tm_sec;

    if (m_curr_second%seconds == 0 && m_curr_second != m_last_second) { // ogni 5 secondi
        m_last_second = m_curr_second;
        return true;
    }
    return false;
}



// true: se ci troviamo nel modulo de secondo richiestso (Sec%reqSec)
bool OnTime_Class::onMinuteModulo(uint8_t minutes) {
    m_timeinfo = rtc.getTimeStruct();
    m_curr_minute = m_timeinfo.tm_min;

    if (m_curr_minute%minutes == 0 && m_curr_minute != m_last_minute) { // ogni 5 minutei
        m_last_minute = m_curr_minute;
        return true;
    }
    return false;
}

