//
// updated by ...: Loreto Notarantonio
// Date .........: 16-08-2025 18.24.32
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ----- library includes
// #include <sendMessageToTelegram.h> // per functions protoype
#include <lnTime_Class.h> // per functions protoype


// ----- project includes
#include "main.h" // per functions protoype
#define __I_AM_WIFI_PROCESS_CPP__
    #include "wifiProcess.h"
#undef __I_AM_WIFI_PROCESS_CPP__




// #############################################################
// # WIFI CALLBACK
// #############################################################
void wifiConnectedCB(arduino_event_id_t event) {
    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        const char *tg_msg="WIFI - CONNECTED";
        LOG_NOTIFY(tg_msg);
        // sendMessageToTelegram(tg_msg, modeTEXT);
        fWifiConnected=true;
    }
    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        const char *tg_msg="WIFI - DISCONNECTED";
        LOG_ERROR(tg_msg);
        // sendMessageToTelegram(tg_msg, modeTEXT);
        fWifiDisconnected=true;
    }
}


// ##########################################################################
// #
// ##########################################################################
void wifiConnectedAction() {
    if (fWifiConnected) {
        fWifiConnected = false;
        LOG_NOTIFY("wifi connected....");
        // char timestamp[16];
        // lnTime.timeStamp(timestamp, sizeof(timestamp));
        // char tg_msg[100];
        // snprintf(tg_msg, 100, "<b>ESP32 - %s</b>%%0ajust CONNECTED", timestamp);
        // LOG_NOTIFY(tg_msg);
        // sendMessageToTelegram(tg_msg, modeHTML);
    }

}



// ##########################################################################
// #
// ##########################################################################
void wifiDisconnectedAction() {
    if (fWifiDisconnected) {
        fWifiDisconnected = false;
        LOG_NOTIFY("wifi disConnected....");
        myWiFiManager.restartScanning();
        // char timestamp[16];
        // lnTime.timeStamp(timestamp, sizeof(timestamp));
        // char tg_msg[100];
        // snprintf(tg_msg, 100, "<b>ESP32 - %s</b>%%0ajust DISCONNECTED", timestamp);
        // LOG_ERROR(tg_msg);
        // sendMessageToTelegram(tg_msg, modeHTML);
    }
}


