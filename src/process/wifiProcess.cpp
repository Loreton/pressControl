//
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 09.29.07
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    <lnLogger_Class.h>
#include <lnTime_Class.h> // per functions protoype


// ----- project includes
#include "main.h" // per functions protoype
#define __I_AM_WIFI_PROCESS_CPP__
    #include "wifiProcess.h"
#undef __I_AM_WIFI_PROCESS_CPP__




// #############################################################
// # WIFI CALLBACK
// # fare poche azioni ma impostare un flag se serve
// #############################################################
void wifiConnectedCB(arduino_event_id_t event) {
    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        LOG_NOTIFY("WiFi_callBack - Connected");
        fWifiConnected=true;
    }
    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        LOG_ERROR("WiFi_callBack - DISCONNECTED");
        fWifiConnected=false;
    }
    fWifiDisconnected = !fWifiConnected;
}


// ##########################################################################
// #
// ##########################################################################
void wifiConnectedAction() {
    if (fWifiConnected) {
        fWifiConnected = false;
        LOG_NOTIFY("WiFi_connAction Connected!");
        // myBot.clearMessage();
        // myBot.addFormattedString("<b>pressControl</b> - %s\n", lnTime.nowTime());
        setTelegramTitle();
        myBot.addFormattedString("WiFi: <b>CONNECTED</b>\n");

        myBot.send();
    }

}



// ##########################################################################
// #
// ##########################################################################
void wifiDisconnectedAction() {
    static uint32_t lastRestartRequest=0;
    if ( fWifiDisconnected && (millis() - lastRestartRequest > 2*60*1000) ) { // 2 minuti
        lastRestartRequest = millis();
        if (myWiFiManager.restart()) {
            LOG_WARN("WiFi_disconnAction....restarting WiFi");
            fWifiDisconnected = false;
        }
    }
}


