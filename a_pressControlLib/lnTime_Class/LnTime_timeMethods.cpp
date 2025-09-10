/*
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 13.39.14
*/


#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "LnTime_Class.h"



// ################################################################
// Converte millisecondi in HH:MM:SS.ms
// ritorna il timestamp del giorno
//    withMilliSec = true: aggiunge .xxx alla fine della stringa
//    stripHeader = true: rimuove hour se == 0
// ################################################################
const char* LnTime_Class::msecToHMS(char *buffer, uint8_t buffer_len, uint32_t millisec, bool withMilliSec, bool stripHours) {
    uint16_t msec;
    uint32_t seconds;

    msec    = (millisec % 1000UL);
    seconds = (millisec / 1000UL);

    uint8_t sec      = (seconds  % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);

    if (withMilliSec) {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec); // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    }
    else {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d", hour, min, sec);  // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.

    }

    if (stripHours && hour == 0)  {
        return buffer+3;
    }

    return buffer;
}


const char* LnTime_Class::msecToHMS(uint32_t millisec, bool withMilliSec, bool stripHours) {
    // const char buff_SIZE = 16;
    char buffer[16];
    return msecToHMS(buffer, sizeof(buffer), millisec, withMilliSec, stripHours);
}


const char* LnTime_Class::secToHMS(uint32_t seconds, bool stripHours) {
    // const char buff_SIZE = 16;
    char buffer[16];
    return msecToHMS(buffer, sizeof(buffer), seconds*1000UL, false, stripHours);
}



// Restituisce i secondi mancanti al prossimo minuto completo
int8_t LnTime_Class::secondsToMinute() {
    m_timeinfo = rtc.getTimeStruct();
    uint32_t sec_of_day = (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec;
    int8_t rest = sec_of_day % 60;
    LOG_NOTIFY("sec_of_day: %d rest: %d", sec_of_day, rest);
    return rest;
}

// Restituisce i secondi del giorno
uint32_t LnTime_Class::millisecOfDay(int offset) {
    m_timeinfo = rtc.getTimeStruct();
    return ( (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec + offset ) * 1000UL + rtc.getMillis();
}
// Restituisce i secondi del giorno
uint32_t LnTime_Class::secondsOfDay(int offset) {
    m_timeinfo = rtc.getTimeStruct();
    return (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec + offset;
}

// Restituisce i minuti del giorno
uint32_t LnTime_Class::minutesOfDay(int offset) {
    m_timeinfo = rtc.getTimeStruct();
    return (m_timeinfo.tm_hour * 60) + m_timeinfo.tm_min + offset;
}

// Restituisce l'epoca (epoch time)
uint32_t LnTime_Class::getEpoch(unsigned long offset) {
    return rtc.getEpoch() + offset;
}

// Restituisce la struttura tm
struct tm LnTime_Class::getTimeStruct(void) {
    return rtc.getTimeStruct();
}

// Nuovo metodo per esporre l'istanza di ESP32Time
ESP32Time& LnTime_Class::getRtcInstance() {
    return rtc;
}



// Allinea l'esecuzione all'inizio del minuto
// void LnTime_Class::alignToMinute() {
//     LOG_DEBUG("waiting for minute o'clock");
//     m_timeinfo = rtc.getTimeStruct();
//     while (m_timeinfo.tm_sec != 0) {
//         delay(50); // Piccolo ritardo per evitare busy-waiting eccessivo
//         m_timeinfo = rtc.getTimeStruct();
//     }
// }


// Attende il cambio di secondo
// int8_t LnTime_Class::waitForSecond() {
//     m_last_second = m_timeinfo.tm_sec; // Usa m_timeinfo che dovrebbe essere aggiornato dall'ultima chiamata a getTimeStruct
//     while (rtc.getTimeStruct().tm_sec == m_last_second) { // Rileggi direttamente da rtc per l'attesa
//         delay(50);
//     }
//     return rtc.getTimeStruct().tm_sec - m_last_second;
// }
