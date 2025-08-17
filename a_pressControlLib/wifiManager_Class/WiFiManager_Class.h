//
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 14.12.52
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

            // --- teniamo conto del tempo di disconnessione. S
            // --- se supera m_maxWifiTimeout allora facciamo il restart dell'ESP32
        uint32_t            m_disconnectionStartTime = 0;
        const uint32_t      m_maxWifiTimeout = 5*60*1000; // Timeout massimo senza connessione (5 minuti)

            // --- Intervallo di scansione della rete (in ms), qui 50 minuti
        const uint32_t      m_scanInterval = 50*60*1000;
        uint32_t            m_lastScanTime = 0;
        bool                m_scanning;

        ConnectCallback     m_onConnectCallback = nullptr;

        MillisTimer m_disconnectedTimer("discTimer");

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
        // void restartScanning(void) { connectToBestNetwork(); };
        bool restart(void);
        bool disconnect(void);
        uint32_t disconnectedElapsed(void) const { return m_disconnectionStartTime; };

        // Imposta la funzione di callback da chiamare in caso di connessione
        void setConnectCallback(ConnectCallback callback) { m_onConnectCallback = callback; }

    private:
        // Scansiona le reti e si connette a quella migliore (RSSI più alto)
        void connectToBestNetwork();
        void checkOutOfService();
        static void handleEvent(arduino_event_id_t event);
    };



















