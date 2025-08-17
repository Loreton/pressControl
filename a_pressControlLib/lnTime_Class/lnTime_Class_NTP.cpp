/*
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 20.20.27
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp
#include <esp_sntp.h>
#include <WiFi.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  NO_MODULE_LOG
#include <lnLogger_Class.h>
#include <lnTimer_Class.h>
#include "lnTime_Class.h"







// ==================   NTP functions ==========================
// ==================   NTP functions ==========================
// ==================   NTP functions ==========================


// li metto globali perché servono per tutti
// sntp_sync_status_t  m_ntpLastState = SNTP_SYNC_STATUS_RESET;
// uint32_t            m_lastNtpAttempt = 0;
// const uint32_t      m_NTP_TIMEOUT_MS = 1*60*1000UL; // Timeout di xx minuti

#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// Costruttore
LnTime_Class::LnTime_Class() {
    // Il costruttore può essere vuoto o inizializzare ESP32Time con valori di default
    // o semplicemente preparare le variabili membro.
    // L'inizializzazione del tempo effettivo verrà fatta in setup().
}

MillisTimer ntpTimer;


const char* sntp_status[] = {
    "SNTP_SYNC_STATUS_RESET",
    "SNTP_SYNC_STATUS_COMPLETED",
    "SNTP_SYNC_STATUS_IN_PROGRESS"
};

// Implementazione del metodo statico
void LnTime_Class::cbSyncTime(struct timeval *tv) {
    uint8_t status = sntp_get_sync_status();
    LOG_NOTIFY("NTP time synched: %d [%s]", status, sntp_status[status]);
}

void LnTime_Class::initNTP(void) {
    // Controlla se il Wi-Fi è connesso prima di avviare l'NTP
    // if (WiFi.status() == WL_CONNECTED && !m_ntp_active) {
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi is connected, initializing NTP.");
        ntpTimer.restart();
        // Imposta la modalità di sincronizzazione
        // sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
        sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);

        // Imposta la callback (ora che è un metodo statico, funziona)
        sntp_set_time_sync_notification_cb(cbSyncTime);

        // Imposta l'intervallo di sincronizzazione
        // sntp_set_sync_interval(12 * 60 * 60 * 1000UL); // 12 ore
        sntp_set_sync_interval(5 * 60 * 1000UL); // xx minuti

        // Imposta i server NTP
        configTime(0, 0, m_ntpServer1, m_ntpServer2, m_ntpServer3);

        // m_ntpLastState = sntp_get_sync_status();
        // m_ntpLastState = SNTP_SYNC_STATUS_RESET;

        // Imposta il fuso orario
        setenv("TZ", EUROPE_ROME_TZ, 1);
        tzset();
        m_ntp_active = true;

        // Allinea l'RTC interno con l'ora NTP
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            rtc.setTimeStruct(timeinfo);
            LOG_INFO("RTC synchronized with NTP time. (waiting for NTP sync status...)");
        }
    } else {
        LOG_WARN("WiFi not connected. Skipping NTP initialization.");
        m_ntp_active = false;
    }
}


/**
 * ref: /home/loreto/.platformio/packages/framework-arduinoespressif32-libs/esp32s3/include/lwip/include/apps/esp_sntp.h
 * @brief Get status of time sync
 *
 * After the update is completed, the status will be returned as SNTP_SYNC_STATUS_COMPLETED.
 * After that, the status will be reset to SNTP_SYNC_STATUS_RESET.
 * If the update operation is not completed yet, the status will be SNTP_SYNC_STATUS_RESET.
 * If a smooth mode was chosen and the synchronization is still continuing (adjtime works), then it will be SNTP_SYNC_STATUS_IN_PROGRESS.
 *
 * @return  SNTP_SYNC_STATUS_RESET: Reset status.
 *          SNTP_SYNC_STATUS_COMPLETED: Time is synchronized.
 *          SNTP_SYNC_STATUS_IN_PROGRESS: Smooth time sync in progress.
 */


void LnTime_Class::update(void) {
    // if (WiFi.status() == WL_CONNECTED && !m_ntp_active) {
    if (WiFi.status() == WL_CONNECTED ) {
        if (!m_ntp_active) {
            LOG_INFO("WiFi is connected. Synchronizing time with NTP server...");
            initNTP(); // Imposta il fuso orario e i server NTP
            // LOG_INFO("NTP time synchronization initiated.");
        }

        if (ntpTimer.hasExpired()) {
            ntpTimer.restart();

            struct tm timeinfo;
            if (!getLocalTime(&timeinfo)) {
                Serial.println("Failed to obtain time");
                return;
            }
            Serial.println("Got the time from NTP");
        }
    }
    // if (WiFi.status() == WL_CONNECTED && !m_ntp_active) {
    //     sntp_sync_status_t status = sntp_get_sync_status();
    //     if (status != SNTP_SYNC_STATUS_IN_PROGRESS && status != SNTP_SYNC_STATUS_COMPLETED) {
    //     initNTP();
    // }
}



// void LnTime_Class::update_(void) {
//     if (WiFi.status() == WL_CONNECTED) {
//         // Ottieni lo stato attuale della sincronizzazione NTP
//         sntp_sync_status_t status = sntp_get_sync_status();
//         if (m_ntpLastState != status) {
//             LOG_NOTIFY("NTP sync status: %d [%s] %d", status, sntp_status[status], status==SNTP_SYNC_STATUS_RESET);
//             m_ntpLastState = status;
//         }

//         if (status != SNTP_SYNC_STATUS_IN_PROGRESS) {
//             uint32_t elapsed = millis() - m_lastNtpAttempt;
//             // Se non è sincronizzato, controlla se è passato il tempo di timeout
//             // LOG_NOTIFY("NTP sync status: %d [%s] (elapsed: %lu)", status, sntp_status[status], elapsed);
//             if (elapsed > m_NTP_TIMEOUT_MS) {
//                 LOG_ERROR("NTP sync failed or timed out. Restarting NTP client.");
//                 sntp_stop();
//                 initNTP(); // Avvia un nuovo tentativo
//                 m_lastNtpAttempt = millis();
//             }
//         }
//     } else {
//         // Se il WiFi è disconnesso, disattiva l'NTP per evitare tentativi inutili
//         if (m_ntp_active) {
//             sntp_stop();
//             m_ntp_active = false;
//             LOG_WARN("WiFi disconnected. NTP stopped.");
//         }
//         m_lastNtpAttempt = millis(); // Resetta il timer per un nuovo tentativo al prossimo reconnect
//     }
// }









