//
// updated by ...: Loreto Notarantonio
// Date .........: 28-08-2025 13.45.51
//




#include <Arduino.h>    // in testa anche per le definizioni dei type




#define __I_AM_MAIN_CPP__   // in testa a tutto
// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
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



#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];




// #############################################################
// #
// #############################################################
void setup() {
    initialMemory = ESP.getFreeHeap();

    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-07 - rel_type: %d", ln_RELEASE_TYPE);

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
    lnTime.setup(10*60); // Chiama il metodo setup della tua istanza di LnTime ed imposta ntpIntervalTimeSync to 10 minuti

    // --- "pins_Initialization.cpp"
    pinsInitialization();

    // --- telegram setup url
    LOG_NOTIFY("initializing TelegramBot_Class");
    myBot.init(Loreto_Esp32_BotToken, pressControl_ChatID_str, "HTML");

    //  ------  calcolo memoria
    finalMemory = ESP.getFreeHeap();
    LOG_TRACE("memoria (bytes): initial=%ld - final=%ld - occupied=%ld", initialMemory, finalMemory, (initialMemory - finalMemory)); // Stima RAM allocata

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
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }


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

    // --- impostati dalla callback
    if (fWifiConnected)    {wifiConnectedAction(); }
    if (fWifiDisconnected) {wifiDisconnectedAction(); }




    // -----------------------------------
    // --- SEND NTP sync message to Telegram
    // -----------------------------------
    if (lnTime.atMinuteModulo(2)) {
        sendNtpTelegramMessage();
    }


    if (lnTime.atMinuteModulo(5)) { // ogni xx minuti...
        // LOG_INFO("controllo della connessione WiFi");
        // if (!myWiFiManager.isConnected()) {
        //     myWiFiManager.restart();
        // }
        // else {
        //     myWiFiManager.connectToBestNetwork(); // rescanning
        // }
    }


    /**
     * Leggi lo stato dello startButton
     * Se è stato rilasciato
    */
    startButton.pressingLevelNotification(startButtonNotificationCB);
    if (startButton.released()) {
        startButtonHandler(startButton.currentPressLevel());
        startButton.reset();
    }




    /**
     * Leggi lo stato della pompa
     * Se è stato rilasciato
    */
    pumpState.pressingLevelNotification(pumpNotificationCB);
    if (pumpState.released()) {
        pumpHandler(&pumpState);
        pumpState.reset();
    }


    /**
     * Leggi lo stato del pressControl
     * Se è stato rilasciato
    */
    pressControl.pressingLevelNotification(pressControlNotificationCB);
    if (pressControl.released()) {
        pressControl.reset();
    }


    // # ------------------------
    // # controllo dello stato dei dispositivi
    // # ------------------------ */
    chackActionStatus();


}

