/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 19.24.14
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp
#include <esp_sntp.h>
#include <WiFi.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "LnTime_Class.h"



// ==================   NTP functions ==========================
// ==================   NTP functions ==========================
// ==================   NTP functions ==========================

#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv


const char* sntp_status[] = {
    "SNTP_SYNC_STATUS_RESET",
    "SNTP_SYNC_STATUS_COMPLETED",
    "SNTP_SYNC_STATUS_IN_PROGRESS"
};

// Implementazione del metodo statico (non posso accedere agli attributi della calsse)
void LnTime_Class::cbSyncTime(struct timeval *tv) {
    uint8_t status = sntp_get_sync_status();
    LOG_NOTIFY("NTP time synched: %d [%s]", status, sntp_status[status]);
}


/**
 * @brief Set the sync interval of SNTP operation
 *
 * Note: SNTPv4 RFC 4330 enforces a minimum sync interval of 15 seconds.
 * This sync interval will be used in the next attempt update time throught SNTP.
 * To apply the new sync interval call the sntp_restart() function,
 * otherwise, it will be applied after the last interval expired.
 *
 * @param interval_ms   The sync interval in ms. It cannot be lower than 15 seconds, otherwise 15 seconds will be set.
 */



// ##########################################
// - aggiorna la variabile NTP_synched
// ##########################################
bool LnTime_Class::checkNtpSynched() {
    // bool NTP_synched = true;
    uint32_t elapsed = millis() - m_lastNtpAttempt;

    // Se non è sincronizzato, controlla se è passato il tempo di timeout
    if (elapsed > m_NTP_SYNC_INTERVAL*2) { // attendiamo il doppio dell'intervallo di SYNC

        // metodo 2
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            Serial.println("Got the time from NTP");
            m_NTP_synched = true;
        }
        else {
            Serial.println("Failed to obtain time");
            m_NTP_synched = false;
        }


    }
    return m_NTP_synched;
}



void LnTime_Class::initNTP(void) {
    // Controlla se il Wi-Fi è connesso prima di avviare l'NTP
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi is connected, initializing NTP.");

        // Imposta la modalità di sincronizzazione
        sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH); // oppure: SNTP_SYNC_MODE_IMMED

        // Imposta la callback (ora che è un metodo statico, funziona)
        sntp_set_time_sync_notification_cb(cbSyncTime);

        // Imposta l'intervallo di sincronizzazione
        sntp_set_sync_interval(m_NTP_SYNC_INTERVAL); // xx minuti
        m_lastNtpAttempt = millis();

        // Imposta i server NTP
        configTime(0, 0, m_ntpServer1, m_ntpServer2, m_ntpServer3);

        // Imposta il fuso orario
        setenv("TZ", EUROPE_ROME_TZ, 1);
        tzset();
        m_ntp_active = true;

        // Allinea l'RTC interno con l'ora NTP
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            rtc.setTimeStruct(timeinfo);
            LOG_INFO("RTC synchronized with NTP time. (waiting for NTP sync status...)");
            m_NTP_synched = true;
        }
    } else {
        LOG_WARN("WiFi not connected. Skipping NTP initialization.");
        m_ntp_active = false;
    }
}


/**
 * ref:
 *      /home/loreto/.platformio/packages/framework-arduinoespressif32-libs/esp32s3/include/lwip/include/apps/esp_sntp.h
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

