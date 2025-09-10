//
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 14.30.15
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>
// #include <esp_wifi.h>
// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <LnTime_Class.h> // per functions protoype


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
            // activeBuzzer.pulse(1000);
            activeBuzzer.blinking(300, 200, 3);
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
    if (fWifiConnected) {
        fWifiConnected = false;
        wifiConnectedMessage();
    }
}




// ##########################################################################
// #
// ##########################################################################
void wifiConnectedMessage() {
    LOG_NOTIFY("WiFi_connAction Connected!");
    myBot.startNewMessage("<b>PressControl\nTime:</b> %s\n", lnTime.now());
    myBot.addFormattedString("<b>WiFi: </b> CONNECTED\n");
    myBot.addFormattedString("<b>SSID: </b> %s\n", WiFi.SSID().c_str() );
    myBot.addFormattedString("<b>BSSID:</b> %s\n", WiFi.BSSIDstr().c_str() );
    myBot.addFormattedString("<b>RSSI: </b> %d\n", WiFi.RSSI() );
    myBot.addFormattedString("<b>IP:   </b> %s\n", WiFi.localIP().toString().c_str() );

    if (lnTime.isNtpSynched()) {
        myBot.addFormattedString("<b>NTP:</b> Synched\n");
    } else {
        myBot.addFormattedString("<b>NTP:</b> OUT of sync\n");
    }

    myBot.send();
    #if 0
    #endif
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


