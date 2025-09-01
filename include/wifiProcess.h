//
// updated by ...: Loreto Notarantonio
// Date .........: 01-09-2025 19.34.16
//

#pragma once
    // #include <Arduino.h>    // in testa anche per le definizioni dei type
    #include <WiFiManager_Class.h>


    #ifdef  __I_AM_WIFI_PROCESS_CPP__

        // #define __LOAD_SSIDS_CPP__
        //     #include "wifiManager_ssid_credentials.h" // caricamento delle networks SSIDs
        // #undef __LOAD_SSIDS_CPP__

        // per salvare l'instante della connessione/disconnessione del WIFI
        bool fWifiConnected = false;
        bool fWifiDisconnected = false;

        // Crea un'istanza della classe WiFiManager
        // WiFiManager_Class myWiFiManager;
        // const uint8_t myNetworksCount = sizeof(myNetworks) / sizeof(myNetworks[0]);

    #else
        extern bool fWifiConnected;
        extern bool fWifiDisconnected;
        // extern WiFiManager_Class myWiFiManager;
        // extern      Network myNetworks[];
        // extern const uint8_t myNetworksCount;
    #endif



    void wifiConnectedCB(arduino_event_id_t event);
    void wifiConnectedAction(void);
    void wifiConnectedMessage(void);
    void wifiDisconnectedAction(void);