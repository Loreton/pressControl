/*
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 17.22.02
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
#include "LnTime_Class.h"


void LnTime_Class::update(void) {
    m_timeinfo = rtc.getTimeStruct(); // lo facciamo qui una volta sola....

    // if (WiFi.status() == WL_CONNECTED ) {
    //     if (!m_ntp_active) {
    //         LOG_INFO("WiFi is connected. Starting NTP client...");
    //         initNTP(); // Imposta il fuso orario e i server NTP
    //     } else if (! checkNtpSynched() ) {
    //         LOG_ERROR("NTP sync failed or timed out. Restarting NTP client.");
    //         sntp_stop();
    //         initNTP(); // Avvia un nuovo tentativo
    //     } else {
    //         m_lastNtpAttempt = millis(); // Resetta il timer per un nuovo tentativo al prossimo reconnect
    //     }
    // }
    // else {
    //     // Se il WiFi è disconnesso, disattiva l'NTP per evitare tentativi inutili
    //     if (m_ntp_active) {
    //         sntp_stop();
    //         m_ntp_active = false;
    //         LOG_WARN("No WiFi available. NTP stopped.");
    //     }
    // }


    // verifichiamo NTP ogni due minuti
    if (m_timeinfo.tm_min % 2 == 0 && m_timeinfo.tm_min != m_last_ntp_update) {
        m_last_ntp_update = m_timeinfo.tm_min;
        updateNtpSyncStatus();
    }

    if (!m_dhmCustomUpdate) {
        LOG_SPEC("Faccio aggiornameti alle ore. minuti e day");
        // // Controlla e gestisci il cambio dei giorni
        if (onDay()) {
            // Reset della mappa delle ore per riattivare i flag (fatto direttaemnte nel metodo)
        }



        // // Controlla e gestisci il cambio delle ore
        if (onHour()) {
            // Reset della mappa delle ore per riattivare i flag (fatto direttaemnte nel metodo)
        }


        // // Controlla e gestisci il cambio di minuto prima di tutto
        if (onMinute()) {
            // Reset della mappa delle ore per riattivare i flag (fatto direttaemnte nel metodo)
        }

    }





    // Esegui l'aggiornamento dei flag dei secondi e dei minuti
    #ifdef LN_TIME_CLASS_SECONDS_VECTOR
        updateAtSecondFlags();
    #endif


    #ifdef LN_TIME_CLASS_MINUTES_VECTOR
        updateAtMinuteFlags();
    #endif


    #ifdef LN_TIME_CLASS_HOURS_VECTOR
        updateAtHourFlags();
    #endif


}

