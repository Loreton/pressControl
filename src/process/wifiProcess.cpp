//
// updated by ...: Loreto Notarantonio
// Date .........: 13-09-2025 16.17.24
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
            activeBuzzer.blinking(300, 200, 3, f.waitForPulseEnding);
            // activeBuzzer.waitForPulseEnding(2000);
        }
    }

    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED ) {
        LOG_ERROR("WiFi_callBack - DISCONNECTED");
        if (!fWifiDisconnected) {
            fWifiConnected=false;
            activeBuzzer.blinking(300, 200, 5, f.waitForPulseEnding);
            // activeBuzzer.waitForPulseEnding(3000);

        }
    }
    fWifiDisconnected = !fWifiConnected;
}






// ##########################################################################
// #
// ##########################################################################
void telegramWifiConnectedMessage() {
    myBot.startNewMessage("<b>PressControl\nTime:</b> %s\n", lnTime.now());
    myBot.addFormattedString("<b>WiFi: </b> CONNECTED\n");
    myBot.addFormattedString("<b>SSID: </b> %s\n", WiFi.SSID().c_str() );
    myBot.addFormattedString("<b>BSSID:</b> %s\n", WiFi.BSSIDstr().c_str() );
    myBot.addFormattedString("<b>RSSI: </b> %d\n", WiFi.RSSI() );
    myBot.addFormattedString("<b>IP:   </b> %s\n", WiFi.localIP().toString().c_str() );


    myBot.addFormattedString("<b>NTP status:</b>\n");
    if (lnTime.isNtpSynched()) {
        myBot.addFormattedString(" Synched - %s\n", lnTime.ntpSyncStatus() );
    } else {
        myBot.addFormattedString(" OUT of sync - %s\n", lnTime.ntpSyncStatus() );
    }


    // if (lnTime.isNtpSynched()) {
    //     myBot.addFormattedString("<b>NTP:</b> Synched\n");
    // } else {
    //     myBot.addFormattedString("<b>NTP:</b> OUT of sync\n");
    // }

    // myBot.addFormattedString("<b>NTP status:</b> %s\n", lnTime.ntpSyncStatus());



    myBot.send();
}



// ##########################################################################
// #
// ##########################################################################
void wifiProcess() {
    if (fWifiConnected) {
        fWifiConnected = false;
        telegramWifiConnectedMessage();
    }

    if (fWifiDisconnected) {
        fWifiDisconnected = false;
    }
}