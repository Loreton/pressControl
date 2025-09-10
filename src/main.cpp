//
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 13.25.05
//




#include <Arduino.h>    // in testa anche per le definizioni dei type




#define __I_AM_MAIN_CPP__   // in testa a tutto
// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <lnGlobalVars.h>
#include <lnSerialRead.h>
#include <LnTime_Class.h>
#include <WiFiManager_Class.h>


// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"
#include "wifiProcess.h"



uint8_t tgMsgLen=0;
size_t initialMemory;
size_t finalMemory;



// #define VERSION_LENGTH 40
char pressControlVersion[40];




// #############################################################
// #
// #############################################################
void setup() {
    initialMemory = ESP.getFreeHeap();

    snprintf(pressControlVersion, sizeof(pressControlVersion), "Version_2025-07 - rel_type: %d", ln_RELEASE_TYPE); // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.

    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    LOG_INFO("%s", pressControlVersion);

    // ------ WiFi
    myWiFiManager.init(loretoNetworks, loretoNetworksCount);
    myWiFiManager.setScanInterval(10*60, 1*60);  //  seconds uint16_t whenConnected=10*60, uint16_t whenNotConnected=1*60)


    // ------ set wifi callBack
    myWiFiManager.setConnectCallback(wifiConnectedCB);

    // ------ set Time

    // --- "pins_Initialization.cpp"
    pinsInitialization();

    // --- telegram setup url
    LOG_NOTIFY("initializing TelegramBot_Classs");
    myBot.init(Loreto_Esp32_BotToken, pressControl_ChatID_str, "HTML");
    LOG_NOTIFY("initializing completed");

    //  ------  calcolo memoria
    finalMemory = ESP.getFreeHeap();
    LOG_TRACE("memoria (bytes): initial=%ld - final=%ld - occupied=%ld", initialMemory, finalMemory, (initialMemory - finalMemory)); // Stima RAM allocata

    lnTime.setup(2*60); // Chiama il metodo setup della tua istanza di LnTime ed imposta ntpIntervalTimeSync to 10 minuti
    lnTime.set_dhmCustomUpdate(true);

}


bool    first_run=true;


// #############################################################
// #
// #############################################################
void loop() {

    /** ----------------
      * Piccolo ritardo per evitare busy-waiting
      * e liberare la CPU per altre attività.
    ---------- */
    delay(10);

    char durationBUFFER[16];
    uint32_t duration;
    uint32_t now=millis();
    bool    actionStatusChanged;

    f10SecondsModulo = lnTime.onModulo(0, 0, 10);
    f30SecondsModulo = lnTime.onModulo(0, 0, 30);
    f2MinutesModulo  = lnTime.onModulo(0, 2, 0);
    f3MinutesModulo  = lnTime.onModulo(0, 3, 0);
    f30MinutesModulo = lnTime.onModulo(0, 30, 0);

    // se metto queste chiamate allora devo impostare: lnTime.set_dhmCustomUpdate(true);
    fonDay = lnTime.onDay();
    fonHour = lnTime.onHour();
    fonMinute = lnTime.onMinute();


    // -----------------------------------
    // ------ refresh dei vari oggetti
    // -----------------------------------
    activeBuzzer.update();
    passiveBuzzer.update();
    pressControlLED.update();
    pumpLED.update();
    pressControlRelay.update();
    magnetoTermicoRelay.update();
    myWiFiManager.update();
    lnTime.update();



    // -----------------------------------
    // --- SEND NTP sync message to Telegram
    // -----------------------------------
    if (fWifiConnected)    {wifiConnectedAction(); }
    if (fWifiDisconnected) {wifiDisconnectedAction(); }



    if (fonHour) {
        if (myWiFiManager.isConnected()) {
            LOG_NOTIFY("onHour Invio del WiFi status su Telegram.");
            wifiConnectedMessage();
        }
    }




    /**
     * Leggi lo stato dello startButton
     * Se è stato rilasciato
    */
    startButton.pressingLevelNotification(startButtonNotificationCB);
    if (startButton.read()) {
        startButtonHandler(&startButton);
    }




    /**
     * Leggi lo stato della pompa
     * Se è stato rilasciato
    */
    pumpState.pressingLevelNotification(pumpNotificationCB);
    if (pumpState.read()) {
        pumpHandler(&pumpState);
    }


    /**
     * Leggi lo stato del pressControl
     * Se è stato rilasciato
    */
    pressControl.pressingLevelNotification(pressControlNotificationCB);
    if (pressControl.read()) {
        pressControlHandler(&pressControl);
    }


    // # ------------------------
    // # controllo dello stato dei dispositivi
    // # ------------------------ */
    chackActionStatus();

    if (first_run) {
        first_run=false;
        LOG_INFO("fine primo loop....");
    }

}

