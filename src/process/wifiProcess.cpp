//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 10.36.56
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
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
        if (!fWifiConnected) {
            fWifiConnected=true;
            activeBuzzer.pulse(1000);
        }
    }

    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED ) {
        LOG_ERROR("WiFi_callBack - DISCONNECTED");
        if (!fWifiDisconnected) {
            fWifiConnected=false;
            activeBuzzer.blinking(300, 200, 5);
        }
    }
    fWifiDisconnected = !fWifiConnected;
}


// ##########################################################################
// #
// ##########################################################################
void wifiConnectedAction() {
    // passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 300, fAscendent); // Scala ascendente, 150ms per nota)
    if (fWifiConnected) {
        fWifiConnected = false;
        LOG_NOTIFY("WiFi_connAction Connected!");
        myBot.startNewMessage("<b>PressControl\nTime: %s</b>\n", lnTime.nowTime());
        myBot.addFormattedString("WiFi: <b>CONNECTED</b>\n");
        myBot.send();

    }

}



// ##########################################################################
// #
// ##########################################################################
void wifiDisconnectedAction() {
    // activeBuzzer.blinking(300, 200, 5); // Scala ascendente, 150ms per nota)
    // passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 300, fDiscendent); // Scala ascendente, 150ms per nota)
        // static uint32_t lastRestartRequest=0;
    if ( fWifiDisconnected) { // 2 minuti
        // if (myWiFiManager.restart()) {
        // LOG_ERROR("WiFi_disconntedAction....nothing");
        fWifiDisconnected = false;
    }
}


