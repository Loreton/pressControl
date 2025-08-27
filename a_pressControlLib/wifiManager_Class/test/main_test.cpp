//
// updated by ...: Loreto Notarantonio
// Date .........: 27-08-2025 18.23.44
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>

#define __I_AM_MAIN_CPP__

#include "lnLogger_Class.h"
#include "lnSerialRead.h" // waitForEnter()
#include "LnTime_Class.h"

#include "WiFiManager_Class.h"



// Crea un'istanza della classe WiFiManager
WiFiManager_Class myWiFiManager;
LnTime_Class LnTime;

// ----------------------------------------------------
// callBack on wifi events
// ----------------------------------------------------
void wifiConnectedCB(arduino_event_id_t event) {
    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        LOG_NOTIFY("CALLBACK_WIFI - CONNECTED");
        // LnTime.initNTP();
    }
    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        LOG_ERROR("CALLBACK_WIFI - DIS-CONNECTED");
        // LnTime.setNtpInactive();
    }
}

// #############################################################
// # WIFI CALLBACK
// #############################################################
// void wifiConnectedCB(arduino_event_id_t event) {
//     if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
//         tgMsgLen = snprintf(tgMessageBuffer, TG_MSG_MAX_SIZE, "WIFI - CONNECTED");
//         LOG_NOTIFY(tgMessageBuffer);
//         sendMessageToTelegram(tgMessageBuffer);
//         LnTime.initNTP();
//     }
//     if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
//         tgMsgLen = snprintf(tgMessageBuffer, TG_MSG_MAX_SIZE, "WIFI - DISCONNECTED");
//         LOG_ERROR(tgMessageBuffer);
//         sendMessageToTelegram(tgMessageBuffer);
//         LnTime.setNtpInactive();

//     }
// }



bool isElapsed() {
    static uint32_t lastElapsed, elapsed;
    elapsed = millis() - lastElapsed;
    if (elapsed%2000 >= 10) { // ogni 2 secondi
        lastElapsed = millis();
        return true;
    }
    return false;
}



void setup() {
    Serial.begin(115200);
    delay(1000);
    lnLog.init();
    // Inizializza la classe
    myWiFiManager.init(loretoNetworks, sizeof(loretoNetworks) / sizeof(loretoNetworks[0]));
    myWiFiManager.setScanInterval(3*60, 1*60);  //  seconds uint16_t whenConnected=10*60, uint16_t whenNotConnected=1*60)

    // myWiFiManager.init(loretoNetworks, loretoNetworksCount);
    myWiFiManager.setConnectCallback(wifiConnectedCB);
    delay(5000); // attendiamo un po...
    LnTime.setup();
}


struct tm timeinfo;
void loop() {
    static bool first_run=true;
    static uint8_t last_second=99;
    uint8_t curr_second;
    // static uint32_t startedMillis;
    char nowTimeBUFFER[16];
    // uint32_t now;
    timeinfo = LnTime.getTimeStruct();
    curr_second = timeinfo.tm_sec;
    // LOG_SPEC("curr_second: %d", curr_second);

    if (first_run) {
        first_run=false;
        // startedMillis=millis();
    }


    // Chiama il metodo loop() della classe per la gestione continua
    myWiFiManager.update();
    LnTime.update();




    // now = millis() - startedMillis;
    // if (curr_second%5 == 0 && curr_second != last_second) { // ogni 5 secondi
    if (LnTime.atSecondModulo(15)) { // ogni 5 secondi
        LOG_SPEC("curr_second: %d", curr_second);
        // last_second = curr_second;
        LOG_NOTIFY("-----------");
        LOG_INFO("nowTime:                %s",         LnTime.nowTime());
        LOG_INFO("nowTime:         0      %s",         LnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER)) );
        LOG_INFO("nowTime:         0      %s",         LnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), 0, fMilliSecondsTrue) );

        LOG_INFO("msecToTimeStamp: millis %s",         LnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), millis(), fMilliSecondsTrue));

        LOG_INFO("msecToTimeStamp: mymsec %s",        LnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), LnTime.millisecOfDay(), fMilliSecondsTrue));
        LOG_NOTIFY("------------");

        myWiFiManager.showCurrentConnection();
    }

    delay(50);
    // waitForEnter();

}
