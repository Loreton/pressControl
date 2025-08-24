//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 20.05.07
//


#include <WiFi.h>

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_LEVEL_INFO
#include <lnLogger_Class.h>
// #undef  LOG_MODULE_NOTIFY
// #include <lnTimer_Class.h>

#include "WiFiManager_Class.h"


/* ---
    - Al momento dell'init viene lanciato la scanning asyncron connectToBestNetwork()
    - nel loop() inseriamo la chiamata al metodo update() il quale verifica lo status dello scanning
    - se lo scanning è completato vengono processate le reti trovate processScanResults()
    - viene cercala la bestRSSI e si tenta la connessione
    - se va a buon fine allora dovrebbe scattare l'handleEvent() dove ho messo la callBack
---- */

// ----------------------------------------------------
// Classe WiFiManager_Class
// ----------------------------------------------------
WiFiManager_Class* WiFiManager_Class::s_instance = nullptr;

// Costruttore
WiFiManager_Class::WiFiManager_Class() {
    s_instance = this;
};

// #####################################################################
// Inizializza il WiFi in modalità Station e si connette
// #####################################################################
void WiFiManager_Class::init(Network* creds, int8_t count) {
    m_networks     = creds;
    m_networkCount = count;
    LOG_INFO("Inizializzazione WiFi...");
    WiFi.mode(WIFI_STA);

    // Registra la funzione di gestione degli eventi WiFi
    WiFi.onEvent(handleEvent);

    // fai partire lo scanning delle reti
    connectToBestNetwork();
}

// #####################################################################
// Inizializza il WiFi in modalità Station e si connette
// #####################################################################
void WiFiManager_Class::restart(void) {
    // disconnect();
    // LOG_INFO("re.Inizializzazione WiFi...");
    // WiFi.mode(WIFI_STA);

    // // Registra la funzione di gestione degli eventi WiFi
    // WiFi.onEvent(handleEvent);
    LOG_ERROR("WiFi restarting....");
    m_scanning = false;
    connectToBestNetwork();
}



// #########################################
// # not used... comodo dall'esterno....
// #########################################
bool WiFiManager_Class::disconnect() {
    // if (millis() - m_disconnectionStartTime > 2*60*1000) { // non facciamo disconnessione troppo spesso
        LOG_NOTIFY("Disconnecting wifi %s", WiFi.SSID());
        WiFi.scanDelete();
        if (WiFi.disconnect()) {
            LOG_INFO("[WIFI] Disconnected");
            delay(100);
        } else {
            LOG_ERROR("WiFi was not Connected!");
        }
        m_scanning = false;
        m_lastScanTime = millis();
        // m_disconnectionStartTime = millis(); // aggiornamento tempo di disconnessione
        // return true;
    // }
    // LOG_WARN("disconnection already in progress.....");
    return false;
}




// #####################################################################
// Funzione da chiamare nel loop principale per monitorare la connessione
// Se non siamo connessi o è il momento di scansionare nuovamente, avvia la scansione
// #####################################################################
void WiFiManager_Class::update() {
    // if (millis() - m_disconnectionStartTime > m_maxWifiTimeout) {
    //     LOG_ERROR("WiFi - too long time (%lu ms) disconnected", m_maxWifiTimeout);
    //     m_scanning = false;
    // }

    uint32_t scanTime = millis() - m_lastScanTime;

    // if ((WiFi.status() != WL_CONNECTED || (millis() - m_lastScanTime > m_scanInterval)) && !m_scanning) {
    if ( (WiFi.status() != WL_CONNECTED || scanTime > m_scanInterval ) && !m_scanning) {
        // LOG_NOTIFY("Tempo di scansione periodica o connessione persa. Avvio scansione...");
        if (WiFi.status() != WL_CONNECTED)   {LOG_ERROR("WiFi - connessione non attiva."); }
        if (scanTime > m_scanInterval)       {LOG_NOTIFY("WiFi - Tempo di scansione periodica (%lu ms) scaduto", m_scanInterval); }
        connectToBestNetwork();
        m_lastScanTime = millis();
    }

    // if (!m_scanning) {
    //     if ( (WiFi.status() != WL_CONNECTED) ) {
    //         LOG_ERROR("WiFi - connessione persa");
    //     }
    //     if (scanTime > m_scanInterval) {
    //         LOG_NOTIFY("WiFi - Tempo di scansione periodica (%lu ms) scaduto", m_scanInterval);
    //     }
    //     // m_disconnectionStartTime = millis();
    //     connectToBestNetwork();
    //     m_lastScanTime = millis();
    // }


    // Se la scansione è in corso, controlla se è terminata
    if (m_scanning) {
        int scanResult = WiFi.scanComplete();
        if (scanResult >= 0) { // La scansione è completata
            m_scanning = false;
            processScanResults(scanResult);
        }
    }

    // else {// if (m_scanning) {
    //     m_disconnectionStartTime = millis(); // aggiorna continuamente
    //     int scanResult = WiFi.scanComplete();
    //     if (scanResult >= 0) { // La scansione è completata
    //         processScanResults(scanResult);
    //     }
    // }
}







/*
// #####################################################################
// Funzione da chiamare nel loop principale per monitorare la connessione
// #####################################################################
void WiFiManager_Class::update() {
    // Se non siamo connessi o è il momento di scansionare nuovamente, avvia la scansione
    // if ((WiFi.status() != WL_CONNECTED || (millis() - m_lastScanTime > m_scanInterval)) && !m_scanning) {
    //     LOG_NOTIFY("Tempo di scansione periodica o connessione persa. Avvio scansione...");
    //     connectToBestNetwork();
    //     m_lastScanTime = millis();
    // }



    bool wifi_action = ( (WiFi.status() != WL_CONNECTED || (millis() - m_lastScanTime > m_scanInterval)) && !m_scanning);
    if (wifi_action) {
        if ( (WiFi.status() != WL_CONNECTED) ) {
            LOG_ERROR("WiFi - connessione persa. Avvio scansione...");
        } else {
            LOG_NOTIFY("WiFi - Tempo di scansione periodica (%lu ms) scaduto. Avvio scansione...", m_scanInterval);
        }
        connectToBestNetwork();
        m_lastScanTime = millis();
    }

    // Se la scansione è in corso, controlla se è terminata
    if (m_scanning) {
        int scanResult = WiFi.scanComplete();
        if (scanResult >= 0) { // La scansione è completata
            m_scanning = false;
            processScanResults(scanResult);
        }
    }
}
*/

/*// #####################################################################
// Funzione da chiamare nel loop principale per monitorare la connessione
// #####################################################################
void WiFiManager_Class::update() {
    // Se non siamo connessi o è il momento di scansionare nuovamente, avvia la scansione
    // if ((WiFi.status() != WL_CONNECTED || (millis() - m_lastScanTime > m_scanInterval)) && !m_scanning) {
    //     LOG_NOTIFY("Tempo di scansione periodica o connessione persa. Avvio scansione...");
    //     connectToBestNetwork();
    //     m_lastScanTime = millis();
    // }

    // Se la scansione è in corso, controlla se è terminata
    if (m_scanning) {
        int scanResult = WiFi.scanComplete();
        if (scanResult >= 0) { // La scansione è completata
            m_scanning = false;
            processScanResults(scanResult);
        }
    }
    else {
        if (WiFi.status() != WL_CONNECTED && !m_disconnectedMsg) {
            LOG_ERROR("WiFi NOT connected.");
            m_disconnectedMsg = true;
        }

    }

}

*/

// #####################################################################
// Controlla se è trascorso troppo tempo senza connessione e riavvia l'ESP32
// #####################################################################
// void WiFiManager_Class::checkOutOfService() {
    // if (millis() - m_disconnectionStartTime > m_maxWifiTimeout) {
    // if (m_disconnectedTimer.hasExpired()) {
    //     LOG_ERROR("disconnection time expired %lu. restarting....", m_maxWifiTimeout);
    //     LOG_ERROR("RESTARTING ESP");
    //     LOG_ERROR("RESTARTING ESP");
    //     LOG_ERROR("RESTARTING ESP");
    //     LOG_ERROR("RESTARTING ESP");
    //     restart();
    //     // ESP.restart();
    // }
// }


// #####################################################################
// Avvia una scansione non bloccante
// #####################################################################
void WiFiManager_Class::connectToBestNetwork() {
    if (!m_scanning) {
        m_scanning = true;
        LOG_NOTIFY("Avvio scansione reti (asincrona)...");
        WiFi.scanNetworks(true); // Scansione non bloccante
    }
}





// #####################################################################
// Elabora i risultati della scansione e si connette alla rete migliore
// #####################################################################
void WiFiManager_Class::processScanResults(int networks) {
    int8_t bestRSSI = -127; // Valore RSSI minimo
    int8_t bestNetworkIndex = -1;

    if (networks == 0) {
        LOG_WARN("Nessuna rete trovata.");
        return;
    }

    LOG_INFO("reti trovate: %d", networks);

    // Cerca la rete migliore tra quelle configurate
    for (int i = 0; i < networks; ++i) {
        LOG_INFO("  %d: SSID: %-12s BSSID: %s (%d dBm)", i + 1, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.RSSI(i));

        for (int j = 0; j < m_networkCount; ++j) {
            if (String(WiFi.SSID(i)) == String(m_networks[j].ssid)) {
                if (WiFi.RSSI(i) > bestRSSI) {
                    bestRSSI = WiFi.RSSI(i);
                    bestNetworkIndex = j;
                }
                break;
            }
        }
    }
    // --- non cambiamo se il gap di livello è inferiore a 10
    int8_t currRSSI = WiFi.RSSI();
    if ( (currRSSI == 0) || (bestRSSI - currRSSI >= 10) ) {
        connectToSSID(bestNetworkIndex);
    } else {
        // LOG_NOTIFY("currSSID: %-12s (%d dBm) - newSSID: %-12s (%d dBm)", WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.SSID().c_str(), WiFi.RSSI(bestNetworkIndex), bestRSSI);
        LOG_NOTIFY("currSSID: %-12s (%d dBm)", WiFi.SSID().c_str(), WiFi.RSSI());
        LOG_NOTIFY("newSSID:  %-12s (%d dBm)", WiFi.SSID(bestNetworkIndex), bestRSSI);  // il nome lo prelevo dalla mia struttura che è char*
        LOG_NOTIFY("RSSI gap is less than 10. Mantaining current SSID");
    }
}





void WiFiManager_Class::connectToSSID(int8_t networkIndex) {
    if (networkIndex == -1) {
        LOG_ERROR("Nessuna delle reti configurate è stata trovata.");
    } else {
        const char *ssid     = m_networks[networkIndex].ssid;
        const char *password = m_networks[networkIndex].password;
        LOG_SPEC("best net: [%d] - %s", networkIndex, ssid);

        // Controlla se siamo già connessi alla rete migliore
        if (WiFi.status() == WL_CONNECTED && String(WiFi.SSID()) == String(ssid)) {
            LOG_NOTIFY("Già connesso alla rete migliore: %s - %s.", WiFi.SSID(), WiFi.BSSIDstr().c_str());
            LOG_NOTIFY("...non è necessario cambiare.");
            m_disconnectedMsg = false;
            m_scanning = false;
        } else {
            LOG_INFO("Connessione a: %s", ssid);
            WiFi.begin(ssid, password);
            m_disconnectedMsg = false;
            m_scanning = false;
        }
    }

}




// Funzione statica per la gestione degli eventi Wi-Fi
void WiFiManager_Class::handleEvent(arduino_event_id_t event) {
    if (s_instance) {
        switch (event) {

            case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                LOG_INFO("Connesso! %s - %s - %s", WiFi.SSID(), WiFi.BSSIDstr().c_str(), WiFi.localIP().toString().c_str());
                // m_disconnectedTime = millis();
                s_instance->m_disconnectionStartTime = millis();
                break;

            case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                LOG_ERROR("WiFi - Connessione persa.");
                // m_disconnectedTime = millis();
                s_instance->m_disconnectionStartTime = millis();
                break;
            // Aggiungi altri eventi se necessario...
        }

        if (s_instance->m_onConnectCallback) {
            s_instance->m_onConnectCallback(event);
        }
    }
}



