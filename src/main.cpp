//
// updated by ...: Loreto Notarantonio
// Date .........: 23-08-2025 18.26.17
//




#include <Arduino.h>    // in testa anche per le definizioni dei type




#define __I_AM_MAIN_CPP__   // in testa a tutto
// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    <lnLogger_Class.h>
#include    <lnGlobalVars.h>
#include    <lnSerialRead.h>
// #include    <functionPrototypes.h>
#include    <lnTime_Class.h>
// #include    <onTime_Class.h>


// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"

#include "wifiProcess.h"





uint8_t tgMsgLen=0;
size_t initialMemory;
size_t finalMemory;

// OnTime_Class mainOnTime;
// OnTime_Class wifiOnTime;



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
    myWiFiManager.init(myNetworks, myNetworksCount);

    // ------ set wifi callBack
    myWiFiManager.setConnectCallback(wifiConnectedCB);

    // ------ set Time
    lnTime.setup(); // Chiama il metodo setup della tua istanza di LnTime

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


    // if (fWifiConnected)    {wifiConnectedAction(); }
    // if (fWifiDisconnected) {wifiDisconnectedAction(); }





    if (lnTime.atMinuteModulo(30)) {
        char timestamp[16];
    }


    if (lnTime.atMinuteModulo(5)) { // ogni xx minuti...
        LOG_INFO("controllo della connessione WiFi");
        if (!myWiFiManager.isConnected()) {
            myWiFiManager.restart();
        }
        else {
            myWiFiManager.connectToBestNetwork(); // rescanning
        }
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


    /* ------------------------
        controllo dello stato dei dispositivi
    ------------------------ */
    chackActionStatus();


    #if 0
    // -----------------------------------
    // ------ Action
    // -----------------------------------
    uint8_t relayStatus = pressControlRelay.isActive();
    uint8_t pumpStatus  = pumpState.isPressed();
    uint8_t pcStatus    = pressControl.isPressed();

    actionState = (pcStatus*2) + (pumpStatus*1);
    actionStatusChanged = (actionState == lastActionState) ? false : true;
    if (fPUMP_ALARM) {
        actionState = pumpAlarm;
    }


    if (actionStatusChanged) { // facciamo comunque il display ogni 15 secondi
        myBot.clearMessage();
        myBot.addFormattedString("<b>pressControl</b> - %s\n", lnTime.nowTime());
        myBot.addFormattedString("PC relay: <b>%s</b>\nPC status: <b>%s</b>\nPUMP status: <b>%s</b>\n", str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);
        myBot.send();
    }

    // if ( (actionState != lastActionState) || (now - lastDisplayTime) > actionStateDisplayInterval) { // facciamo comunque il display ogni 15 secondi
    if ( actionStatusChanged || (now - lastDisplayTime) > actionStateDisplayInterval) { // facciamo comunque il display ogni 15 secondi
        lastActionState=actionState;
        lastDisplayTime=now;

        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);

    }








    switch (actionState) {

        // status normale in attesa che si accenda il PC
        case pumpAlarm:
            LOG_ERROR("Pump Alarm");
            startAlarmActions();
            break;


        // status normale in attesa che si accenda il PC
        case pcOFF_pumpOFF:
            if (relayStatus) {
                // non può essere il rele on ed il PC off
                LOG_ERROR("Relè OFF quando invece il PC è ON");
                startAlarmActions();
            } else {
                // LOG_INFO("Situazione normale");
                resetAlarmActions();
            }
            break;


        // non può essere la pompa ON ed il PC off
        case pcOFF_pumpON:
            LOG_ERROR("Pump ON quando il PC è OFF.");
            startAlarmActions();
            break;

        // status normale in attesa che si accenda la pompa
        case pcON_pumpOFF:
            actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;
            pressControlLED.on();         // accendiamo fisso il LED
            pumpLED.off(); // facciamoòp lampeggiare
            break;


        // status normale con la pompa accesa
        case pcON_pumpON:
            actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;
            pressControlLED.on();
            pumpLED.on();
            break;

        default:
            break;

    } // endo of switch

    #endif

}

    #if 0


// #############################################################
// #
// #############################################################
void startAlarmActions() {
    static uint32_t relay_delay=MAGNETOTERMIC_RELAY_PULSETIME + 3000;
    static uint32_t last_relay_time;
    if (! fPUMP_ALARM) {
        LOG_ERROR("Starting Recovery Actions for Alarm."); // NO perchè compare ad ogni giro di loop
        last_relay_time=0;
    }

    fPUMP_ALARM=true;
    pressControlRelay.off();
    // tentiamo di togliere corrente al magnetotermico
    if ( (millis() - last_relay_time) > relay_delay ) {
        magnetoTermicoRelay.startPulse(MAGNETOTERMIC_RELAY_PULSETIME); // non abbiamo armi. proviamo a chiudere il relay esterno togliendogli alimentazione
        last_relay_time = millis();
    }
    pumpLED.blinking(300, 300);
    pressControlLED.blinking(300, 300);
    activeBuzzer.blinking(300, 300);
    actionStateDisplayInterval=2000; // ogni due secondi
}



// #############################################################
// #
// #############################################################
void resetAlarmActions() {
    if (fPUMP_ALARM) {
        LOG_INFO("Recovery Actions for Alarm Ended."); // NO perchè compare ad ogni giro di loop
        fPUMP_ALARM=false;
    }
    actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;
    pressControlLED.blinking(1000, 3000);
    pumpLED.blinking(1000, 3000);
    activeBuzzer.reset();
    passiveBuzzer.myNoTone();
    pressControlRelay.off();       // spegniamo epr sicurezza il relay interno
    magnetoTermicoRelay.off();
}

    #endif