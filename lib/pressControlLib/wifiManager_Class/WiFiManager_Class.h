//
// updated by ...: Loreto Notarantonio
// Date .........: 12-08-2025 17.41.56
//


#pragma once
    #include <WiFi.h>
    #include <esp_wifi.h>

    #include "wifiManager_ssid_credentials.h"


    // CallBack per gestire connessione e disconnessione
    // passiamo come parametro leventID (WiFiGeneric.h)
    // vedi funzione handleEvent()
    // mi è utile per attivare NTP o meno
    typedef void (*ConnectCallback)(arduino_event_id_t event);

    // ----------------------------------------------------
    // Classe WiFiManager_Class
    // ----------------------------------------------------
    class WiFiManager_Class {
    private:
        Network*            m_networks;
        int8_t              m_networkCount;
        bool                m_neverConnected=true;
        bool                m_scanning;
        uint32_t            m_lastScanTime = 0;
        uint32_t            m_wifiOutTime = 0;
        const uint32_t      m_scanInterval = 50*60*1000; // Intervallo di scansione (in ms), qui 50 minuti
        const uint32_t      m_maxWifiTimeout = 1*60*60*1000; // Timeout massimo senza connessione (1 ora)

        ConnectCallback     m_onConnectCallback = nullptr;

        static WiFiManager_Class* s_instance; // Istanza statica per la gestione degli eventi

    public:
        // Costruttore
        WiFiManager_Class(void);

        // Inizializza il WiFi in modalità Station e si connette
        void init(Network* creds, int8_t count);

        // Funzione da chiamare nel loop principale per monitorare la connessione
        void update();
        void processScanResults(int n);
        bool isConnected(void) {return WiFi.status() == WL_CONNECTED; };

        // Imposta la funzione di callback da chiamare in caso di connessione
        void setConnectCallback(ConnectCallback callback) { m_onConnectCallback = callback; }

    private:
        // Scansiona le reti e si connette a quella migliore (RSSI più alto)
        void connectToBestNetwork();
        void checkOutOfService();
        static void handleEvent(arduino_event_id_t event);
    };



















