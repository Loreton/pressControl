//
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 17.43.56
//


#include <WiFi.h>

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  NO_MODULE_LOG
#include <lnLogger_Class.h>
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

    // creazione timer di comodo
    // MillisTimer _disconnectedTimer("discTimer"); // Crea un'istanza della classe


    // Registra la funzione di gestione degli eventi WiFi
    WiFi.onEvent(handleEvent);
    m_disconnectedTimer.init("disconnetedWiFi", m_maxWifiTimeout); // initialize timer

    connectToBestNetwork();
}

// #####################################################################
// Inizializza il WiFi in modalità Station e si connette
// #####################################################################
bool WiFiManager_Class::restart(void) {
    if (disconnect()) {
        LOG_INFO("re.Inizializzazione WiFi...");
        WiFi.mode(WIFI_STA);

        // Registra la funzione di gestione degli eventi WiFi
        WiFi.onEvent(handleEvent);

        connectToBestNetwork();
        return true;
    };
    return false;
}



// #########################################
// # not used... comodo dall'esterno....
// #########################################
bool WiFiManager_Class::disconnect() {
    if (millis() - m_disconnectionStartTime > 2*60*1000) { // non facciamo disconnessione troppo spesso
        LOG_NOTIFY("Disconnecting wifi %s", WiFi.SSID());
        WiFi.scanDelete();
        if (WiFi.disconnect()) {
            LOG_INFO("[WIFI] Disconnected");
            delay(100);
        } else {
            LOG_ERROR("Error Disconnecting");
        }
        m_scanning = false;
        m_lastScanTime = millis();
        m_disconnectionStartTime = millis(); // aggiornamento tempo di disconnessione
        return true;
    }
    LOG_WARN("disconnection already in progress.....");
    return false;
}




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


    // Se la scansione è in corso, controlla se è terminata
    if (m_scanning) {
        int scanResult = WiFi.scanComplete();
        if (scanResult >= 0) { // La scansione è completata
            m_scanning = false;
            processScanResults(scanResult);
        }
    }
    else {
        if (WiFi.status() != WL_CONNECTED) {
            LOG_ERROR("WiFi NOT connected. Starting scanNetwork...");
            // if (m_disconnectionStartTime == 0) {
            //     m_disconnectionStartTime = millis(); // aggiornamento tempo di disconnessione
            // }
            if (!m_disconnectedTimer.isRunning()) {
                m_disconnectedTimer.restart();
            }
        }

        else if (millis() - m_lastScanTime > m_scanInterval) {
            LOG_NOTIFY("Tempo di scansione periodica scaduto (%lu ms). Starting scanNetwork...", m_scanInterval);
        }
        connectToBestNetwork();
        m_lastScanTime = millis();

    }

    if (m_disconnectedTimer.hasExpired()) {
        checkOutOfService();
    }
}



// #####################################################################
// Controlla se è trascorso troppo tempo senza connessione e riavvia l'ESP32
// #####################################################################
void WiFiManager_Class::checkOutOfService() {
    // if (millis() - m_disconnectionStartTime > m_maxWifiTimeout) {
    if (m_disconnectedTimer.hasExpired()) {
        LOG_ERROR("disconnection time expired %lu. restarting....", m_maxWifiTimeout);
        LOG_ERROR("RESTARTING ESP");
        LOG_ERROR("RESTARTING ESP");
        LOG_ERROR("RESTARTING ESP");
        LOG_ERROR("RESTARTING ESP");
        restart();
        // ESP.restart();
    }
}


// #####################################################################
// Avvia una scansione non bloccante
// #####################################################################
void WiFiManager_Class::connectToBestNetwork() {
    if (!m_scanning) {
        LOG_NOTIFY("Avvio scansione reti (asincrona)...");
        WiFi.scanNetworks(true); // Scansione non bloccante
        m_scanning = true;
    }
}





// #####################################################################
// Elabora i risultati della scansione e si connette alla rete migliore
// #####################################################################
void WiFiManager_Class::processScanResults(int n) {
    int bestRSSI = -127; // Valore RSSI minimo
    int bestNetworkIndex = -1;

    if (n == 0) {
        LOG_WARN("Nessuna rete trovata.");
        return;
    }

    LOG_INFO("reti trovate: %d", n);

    // Cerca la rete migliore tra quelle configurate
    for (int i = 0; i < n; ++i) {
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

    // Connessione alla rete scelta
    if (bestNetworkIndex != -1) {
        // Controlla se siamo già connessi alla rete migliore
        if (WiFi.status() == WL_CONNECTED && String(WiFi.SSID()) == String(m_networks[bestNetworkIndex].ssid)) {
            LOG_NOTIFY("Già connesso alla rete migliore: %s - %s.", WiFi.SSID(), WiFi.BSSIDstr().c_str());
            LOG_NOTIFY("...non è necessario cambiare.");
            // m_disconnectionStartTime = 0; // aggiornamento tempo di disconnessione
            m_disconnectedTimer.stop();
        } else {
            LOG_INFO("Connessione a: %s", m_networks[bestNetworkIndex].ssid);
            WiFi.begin(m_networks[bestNetworkIndex].ssid, m_networks[bestNetworkIndex].password);
            // m_disconnectionStartTime = 0; // aggiornamento tempo di disconnessione
            m_disconnectedTimer.stop();
        }
    } else {
        LOG_ERROR("Nessuna delle reti configurate è stata trovata.");
    }
}


// Funzione statica per la gestione degli eventi Wi-Fi
void WiFiManager_Class::handleEvent(arduino_event_id_t event) {
    if (s_instance) {
        switch (event) {

            case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                LOG_INFO("Connesso! %s - %s - %s", WiFi.SSID(), WiFi.BSSIDstr().c_str(), WiFi.localIP().toString().c_str());
                // s_instance->m_disconnectionStartTime = 0; // Resetta il timer di timeout
                s_instance->m_disconnectedTimer.stop(); // Resetta il timer di timeout
                break;

            case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                LOG_ERROR("WiFi - Connessione persa.");
                // s_instance->m_disconnectionStartTime = millis(); // Resetta il timer di timeout
                s_instance->m_disconnectedTimer.restart(); // Resetta il timer di timeout
                // s_instance->m_scanning = false; // to restart connection
                break;
            // Aggiungi altri eventi se necessario...
        }

        if (s_instance->m_onConnectCallback) {
            s_instance->m_onConnectCallback(event);
        }
    }
}



