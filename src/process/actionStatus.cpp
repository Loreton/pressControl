//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 07.49.35
//



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
#include    <lnLogger_Class.h>
#include    <lnGlobalVars.h>
#include    <lnTime_Class.h>


// ---------------------------------
// - project headers files
// ---------------------------------
#include "functionPrototypes.h"
#include "main.h"



// Definisce i possibili tipi di condizioni
enum ActionState : uint8_t {
    pcOFF_pumpOFF = 0,      // tutto spento.
    pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    pcON_pumpON,     // rele esterno - Pressione lunga.
    pumpAlarm,
} ;


uint32_t actionStateDisplayInterval = ACTION_STATUS_DISPLAY_INTERVAL;



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



// #############################################################
// #
// #############################################################
void chackActionStatus() {
    static uint8_t  lastActionState     = 1;
    static uint32_t lastDisplayTime     = 0;
    uint32_t now                        = millis();

    bool    actionStateChanged;
    uint8_t actionState;

    // -----------------------------------
    // ------ Action
    // -----------------------------------
    uint8_t relayStatus = pressControlRelay.isActive();
    uint8_t pumpStatus  = pumpState.isPressed();
    uint8_t pcStatus    = pressControl.isPressed();

    actionState = (pcStatus*2) + (pumpStatus*1);
    actionStateChanged = (actionState == lastActionState) ? false : true;
    if (fPUMP_ALARM) {
        actionState = pumpAlarm;
    }


    if (actionStateChanged) { // facciamo comunque il display ogni 15 secondi
        myBot.clearMessage();
        myBot.addFormattedString("<b>pressControl</b> - %s\n", lnTime.nowTime());
        myBot.addFormattedString("PC relay: <b>%s</b>\nPC status: <b>%s</b>\nPUMP status: <b>%s</b>\n", str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);
        myBot.send();
    }

    // if ( (actionState != lastActionState) || (now - lastDisplayTime) > actionStateDisplayInterval) { // facciamo comunque il display ogni 15 secondi
    if ( actionStateChanged || (now - lastDisplayTime) > actionStateDisplayInterval) { // facciamo comunque il display ogni 15 secondi
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


}


