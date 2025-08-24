//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 19.34.21
//


#pragma once
    #include <WiFi.h>
    #include <esp_wifi.h>



    // ---------------------------------
    // lnLibrary headers files
    // ---------------------------------
    // #include "lnTimer_Class.h"
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
            const uint32_t      m_maxWifiTimeout = 1*60*1000UL; // Timeout massimo senza connessione (5 minuti)
            // uint32_t            m_disconnectedTime = 1*60*1000UL; // tempo di disconnessione.... 1 minuto

                // --- Intervallo di scansione della rete (in ms), qui 50 minuti
            const uint32_t      m_scanInterval = 5*60*1000;
            uint32_t            m_lastScanTime = 0;
            bool                m_scanning;
            bool                m_disconnectedMsg = false;

            ConnectCallback     m_onConnectCallback = nullptr;
            // MillisTimer         m_disconnectedTimer;


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
            void restart(void);
            bool disconnect(void);
            uint32_t disconnectedElapsed(void) const { return m_disconnectionStartTime; };

            // Imposta la funzione di callback da chiamare in caso di connessione
            void setConnectCallback(ConnectCallback callback) { m_onConnectCallback = callback; }
            void connectToBestNetwork();

        private:
            // Scansiona le reti e si connette a quella migliore (RSSI più alto)
            void checkOutOfService();
            void connectToSSID(int8_t networkIndex=-1);
            static void handleEvent(arduino_event_id_t event);
    };



















