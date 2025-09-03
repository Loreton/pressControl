/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 12.11.41
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
#include <lnTimer_Class.h>
#include "LnTime_Class.h"


// ----------------------------------------------------
// Classe LnTime_Class
// ----------------------------------------------------
LnTime_Class lnTime;

// ==================   TIME functions ==========================


// ################################################################
// # ntpIntervalTimeSync: seconds
// ################################################################
void LnTime_Class::setup(uint16_t ntpIntervalTimeSync) {
    m_NTP_SYNC_INTERVAL = ntpIntervalTimeSync*1000UL;
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi is connected. Synchronizing time with NTP server...");
        initNTP(); // Imposta il fuso orario e i server NTP
        LOG_INFO("NTP time synchronization initiated.");
    }
    else {
        rtc.setTime(0, 0, 8, 1, 6, 2025); // 1st Jun 2025 08:00:00 - Esempio di data iniziale
        configTzTime(EUROPE_ROME_TZ,  m_ntpServer1, m_ntpServer2, m_ntpServer3);
        LOG_INFO("RTC local time set");
        m_ntp_active = false;
    }
}







// Ottiene l'ora corrente formattata HH:MM:SS
char *LnTime_Class::nowTime() {
    m_timeinfo = rtc.getTimeStruct();
    snprintf(sharedTimeBUFFER, sizeof(sharedTimeBUFFER), "%02d:%02d:%02d", m_timeinfo.tm_hour, m_timeinfo.tm_min, m_timeinfo.tm_sec);
    return sharedTimeBUFFER;
}






// ################################################################
// Converte millisecondi in HH:MM:SS.ms
// ritorna il timestamp del giorno
//    se millisec != 0 allora converte i millisec in timestamp
//    withMilliSec = true: aggiunge .xxx alla fine della stringa
//    stripHeader = true: rimuove hour o minutes se == 0
// ################################################################
const char* LnTime_Class::toHMS(char *buffer, uint8_t buffer_len, uint32_t millisec, bool withMilliSec, bool stripHours) {
    uint16_t msec;
    uint32_t seconds;

    if (millisec == 0) {
        m_timeinfo = rtc.getTimeStruct();
        seconds = (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec;
        msec = rtc.getMillis();  // current mSeconds (0-999)
    }
    else {
        msec    = (millisec % 1000UL);
        seconds = (millisec / 1000UL);
    }

    uint8_t sec      = (seconds  % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);

    if (withMilliSec) {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    }
    else {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d", hour, min, sec);
    }

    if (stripHours && hour == 0)  {
        return buffer+3;
    }

    return buffer;
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

// ######################################################################
// # Controlla se è iniziato un nuovo secondo
// ######################################################################
bool LnTime_Class::onSecond() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_sec != m_last_second) {
        m_last_second = m_timeinfo.tm_sec;
        return true;
    }
    return false;
}


// ######################################################################
// # true se ci troviamo al secondo x
// # lavorando sull'epoch time possiamo superare anche il 60
// ######################################################################
bool LnTime_Class::atSecond(int8_t second) {
    m_timeinfo = rtc.getTimeStruct();

    // Trova l'ultimo secondo registrato per questo valore "second".
    // Se non esiste, il valore di default è -1 (o un valore che non si ripete mai) per la prima esecuzione.
    int8_t last_at_second = -1;
    if (m_at_last_second_map.count(second)) {
        last_at_second = m_at_last_second_map[second];
    }

    if (m_timeinfo.tm_sec == second && m_timeinfo.tm_sec != last_at_second) {
        m_at_last_second_map[second] = m_timeinfo.tm_sec;
        return true;
    }
    return false;
}





// ######################################################################
// # true se ci troviamo al minuto mod (x)
// # lavorando sull'epoch time possiamo superare anche il 60
// ###############################################################
bool LnTime_Class::onSecondModulo(uint32_t modulo, bool trueOnFirstRun) {
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


// // ######################################################################
// // # true se ci troviamo al secondo mod (x)
// // # lavorando sull'epoch time possiamo superare anche il 60
// // ######################################################################
// bool LnTime_Class::onSecondModulo(uint32_t modulo) {
//     uint32_t current_epoch = rtc.getEpoch();
//     uint32_t last_epoch = 0;

//     if (modulo == 0) {modulo = 60; }

//     // Controlla se il modulo esiste già nella mappa. Se no, usa 0 come valore iniziale.
//     if (m_last_epoch_seconds_map.count(modulo)) {
//         last_epoch = m_last_epoch_seconds_map[modulo];
//     }

//     if (current_epoch != last_epoch && current_epoch % modulo == 0) {
//         m_last_epoch_seconds_map[modulo] = current_epoch; // Aggiorna la mappa per questo modulo
//         return true;
//     }
//     return false;
// }



// ######################################################################
// # true se ci troviamo al nuovo minuto)
// ######################################################################
bool LnTime_Class::onMinute() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_sec == 0 && m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;
        return true;
    }
    return false;
}

// ######################################################################
// # true se ci troviamo al minuto (x)
// ######################################################################
bool LnTime_Class::atMinute(int8_t minute) {
    m_timeinfo = rtc.getTimeStruct();

    // Trova l'ultimo minuto registrato per questo valore "minute".
    int8_t last_at_minute = -1;
    if (m_at_last_minute_map.count(minute)) {
        last_at_minute = m_at_last_minute_map[minute];
    }

    if (m_timeinfo.tm_sec == 0 && m_timeinfo.tm_min == minute && m_timeinfo.tm_min != last_at_minute) {
        m_at_last_minute_map[minute] = m_timeinfo.tm_min;
        return true;
    }
    return false;
}




// ######################################################################
// # true se ci troviamo al minuto mod (x)
// # lavorando sull'epoch time possiamo superare anche il 60
// ###############################################################
bool LnTime_Class::onMinuteModulo(uint32_t modulo, bool trueOnFirstRun) {
    bool firstRun = true;
    bool isTime = false;
    if (modulo == 0) {modulo = 60; }

    uint32_t current_epoch_minutes = rtc.getEpoch() / 60;

    uint32_t last_epoch_minutes = 0;
    if (m_last_epoch_minutes_map.count(modulo)) { // se esiste la entry
        last_epoch_minutes = m_last_epoch_minutes_map[modulo]; // prelevane il valore
        firstRun = false;
    }


    if (current_epoch_minutes != last_epoch_minutes && current_epoch_minutes % modulo == 0) {
        m_last_epoch_minutes_map[modulo] = current_epoch_minutes; // Aggiorna la mappa
        isTime = true;
    }

    if (!trueOnFirstRun) {firstRun=false; } // se non è stato richiesto il trueOnFirstRun

    return (isTime || firstRun) ? true : false;
}




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



// Attende il cambio di secondo
// int8_t LnTime_Class::waitForSecond() {
//     m_last_second = m_timeinfo.tm_sec; // Usa m_timeinfo che dovrebbe essere aggiornato dall'ultima chiamata a getTimeStruct
//     while (rtc.getTimeStruct().tm_sec == m_last_second) { // Rileggi direttamente da rtc per l'attesa
//         delay(50);
//     }
//     return rtc.getTimeStruct().tm_sec - m_last_second;
// }

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




// Implementazioni delle funzioni di stampa (possono essere in un nuovo file .cpp, es. LnTime_ClassUtils.cpp)
void printLocalTime(const struct tm *timeinfo_ptr) {

#if LOG_LEVEL >= LOG_LEVEL_TRACE
    char buffer[64];

    strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", timeinfo_ptr);
    LOG_SPEC("%s", buffer);

    strftime(buffer, sizeof(buffer), "%A", timeinfo_ptr);
    LOG_SPEC("Day of week: %s", buffer);

    strftime(buffer, sizeof(buffer), "%B", timeinfo_ptr);
    LOG_SPEC("Month: %s", buffer);

    strftime(buffer, sizeof(buffer), "%d", timeinfo_ptr);
    LOG_SPEC("Day of Month: %s", buffer);

    strftime(buffer, sizeof(buffer), "%Y", timeinfo_ptr);
    LOG_SPEC("Year: %s", buffer);

    strftime(buffer, sizeof(buffer), "%H", timeinfo_ptr);
    LOG_SPEC("Hour: %s", buffer);

    strftime(buffer, sizeof(buffer), "%I", timeinfo_ptr);
    LOG_SPEC("Hour (12 hour format): %s", buffer);

    strftime(buffer, sizeof(buffer), "%M", timeinfo_ptr);
    LOG_SPEC("Minute: %s", buffer);

    strftime(buffer, sizeof(buffer), "%S", timeinfo_ptr);
    LOG_SPEC("Second: %s", buffer);

    LOG_SPEC("      Time variables");
    char timeHour[3]; strftime(timeHour, 3, "%H", timeinfo_ptr); LOG_SPEC("%s", timeHour);
    char timeWeekDay[10]; strftime(timeWeekDay, 10, "%A", timeinfo_ptr); LOG_SPEC("%s", timeWeekDay);
    LOG_SPEC("");
#endif
}

void print_rtc_time(LnTime_Class *p) {
    if (!p) return; // Controllo per puntatore nullo

#if LOG_LEVEL >= LOG_LEVEL_DEBUG

    ESP32Time& rtc_instance = p->getRtcInstance(); // Ottieni il riferimento all'istanza rtc

    LOG_NOTIFY("\t%-20s: %s", "getDate(short)",      rtc_instance.getDate(false).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getDate(long)",       rtc_instance.getDate(true).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getTime()",           rtc_instance.getTime());
    LOG_NOTIFY("\t%-20s: %s", "getDateTime(short)",  rtc_instance.getDateTime(false).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getDateTime(long)",   rtc_instance.getDateTime(true).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getTimeDate(short)",  rtc_instance.getTimeDate(false).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getTimeDate(long)",   rtc_instance.getTimeDate(true).c_str());

    LOG_NOTIFY("\t%-20s: %lu", "getEpoch()",          rtc_instance.getEpoch());
    LOG_NOTIFY("\t%-20s: %d",  "getHour()",           rtc_instance.getHour());
    LOG_NOTIFY("\t%-20s: %d",  "getMinute()",         rtc_instance.getMinute());
    LOG_NOTIFY("\t%-20s: %d",  "getSecond()",         rtc_instance.getSecond());
    LOG_NOTIFY("\t%-20s: %lu", "getMillis()",         rtc_instance.getMillis());
    LOG_NOTIFY("\t%-20s: %lu", "getMicros()",         rtc_instance.getMicros());

    LOG_NOTIFY("\t%-20s: %d",  "getHour(true)",       rtc_instance.getHour(true));
    LOG_NOTIFY("\t%-20s: %s",  "getAmPm(uppercase)",  rtc_instance.getAmPm(false).c_str());
    LOG_NOTIFY("\t%-20s: %s",  "getAmPm(lowcase)",    rtc_instance.getAmPm(true).c_str());

    LOG_NOTIFY("\t%-20s: %d",  "getYear()",           rtc_instance.getYear());
    LOG_NOTIFY("\t%-20s: %d",  "getMonth()",          rtc_instance.getMonth());
    LOG_NOTIFY("\t%-20s: %d",  "getDay()",            rtc_instance.getDay());
    LOG_NOTIFY("\t%-20s: %d",  "getDayofWeek()",      rtc_instance.getDayofWeek());
    LOG_NOTIFY("\t%-20s: %d",  "getDayofYear()",      rtc_instance.getDayofYear());
    LOG_NOTIFY("\t%-20s: %lu", "getLocalEpoch()",     rtc_instance.getLocalEpoch());
    LOG_NOTIFY("\t%-20s: %s",  "getAmPm()",           rtc_instance.getTime("%A, %B %d %Y %H:%M:%S").c_str());

    struct tm timeinfo_local = rtc_instance.getTimeStruct();
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo_local);
    LOG_NOTIFY("\t%-20s: %s", "time struct", buffer);
#endif
}



// Crea un'istanza della classe LnTime
// visibile da tutti. Se ne serve una dedicata la posso creare nel modulo
// LnTime_Class lnTime;

