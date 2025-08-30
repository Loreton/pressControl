//
// updated by ...: Loreto Notarantonio
// Date .........: 30-08-2025 18.03.54
//



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_TRACE
#include    <lnLogger_Class.h>
#include    <lnGlobalVars.h>
#include    <LnTime_Class.h>


// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"


// --- module variables
uint8_t relayStatus;
uint8_t pumpStatus;
uint8_t pcStatus;
// bool   fidleStatus;

// Definisce i possibili tipi di condizioni
enum ActionState : uint8_t {
    pcOFF_pumpOFF = 0,      // tutto spento.
    pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    pcON_pumpON,     // rele esterno - Pressione lunga.
    pumpAlarm,
} ;



void sendStatusToTelegram() {
    setTelegramTitle();
    myBot.addFormattedString("<b>Relay:</b> %s\n", str_OnOff[relayStatus]);
    myBot.addFormattedString("<b>PressControl:</b> %s\n", str_OnOff[pcStatus]);
    myBot.addFormattedString("<b>PUMP:</b> %s\n", str_OnOff[pumpStatus]);
    myBot.send();
}



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
    // fSyncBlinking=false;
    pressControlRelay.off();
    // tentiamo di togliere corrente al magnetotermico
    if ( (millis() - last_relay_time) > relay_delay ) {
        magnetoTermicoRelay.startPulse(MAGNETOTERMIC_RELAY_PULSETIME); // non abbiamo armi. proviamo a chiudere il relay esterno togliendogli alimentazione
        last_relay_time = millis();
    }
    pumpLED.blinking(300, 300);
    pressControlLED.blinking(300, 300);
    activeBuzzer.blinking(300, 300);
    // actionStateDisplayInterval=2000; // ogni due secondi
}




// #############################################################
// #
// #############################################################
void resetAlarmActions(bool syncBlinking) {
    if (fPUMP_ALARM) {
        LOG_INFO("Recovery Actions for Alarm Ended."); // NO perchè compare ad ogni giro di loop
        fPUMP_ALARM=false;
    }
    activeBuzzer.reset();
    passiveBuzzer.myNoTone();
    pressControlRelay.off();       // spegniamo epr sicurezza il relay interno
    magnetoTermicoRelay.off();

    if (syncBlinking) {
        LOG_SPEC("Synching LED blinking"); // NO perchè compare ad ogni giro di loop
        pressControlLED.reset(); // per sincronizzare i led
        pumpLED.reset(); // per sincronizzare i led
    }
    pressControlLED.blinking(1000, 3000);
    pumpLED.blinking(1000, 3000);
}



// #############################################################
// #
// #############################################################
void chackActionStatus() {
    static bool fIdleStatus = false;

    static  uint8_t  lastActionState = 1;
            uint32_t now = millis();

            bool    actionStateChanged;
            uint8_t actionState;


    // -----------------------------------
    // ------ Action
    // -----------------------------------
    relayStatus = pressControlRelay.isActive();
    pumpStatus  = pumpState.isPressed();
    pcStatus    = pressControl.isPressed();

    actionState = (pcStatus*2) + (pumpStatus*1);
    if (fPUMP_ALARM) {
        actionState = pumpAlarm;
    }
    actionStateChanged = (actionState == lastActionState) ? false : true;


    if (actionStateChanged) { // facciamo comunque il display ogni 15 secondi
        sendStatusToTelegram();
    }

    if ( actionStateChanged || fModulo2minutes ) { // facciamo comunque il display ogni 15 secondi
        lastActionState=actionState;
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);

    }



    switch (actionState) {

        // status normale in attesa che si accenda il PC
        case pumpAlarm:
            if ( fModulo10Seconds ) {
                LOG_ERROR("Pump Alarm");
                setTelegramTitle();
                myBot.addFormattedString("<b>Pump Alarm!!!!:</b>\n");
                myBot.addFormattedString("<b>Relay:</b> %s\n", str_OnOff[relayStatus]);
                myBot.addFormattedString("<b>PressControl:</b> %s\n", str_OnOff[pcStatus]);
                myBot.addFormattedString("<b>PUMP:</b> %s\n", str_OnOff[pumpStatus]);
                myBot.send();
            }
            fIdleStatus=false;
            startAlarmActions();
            break;


        // status normale in attesa che si accenda il PC
        case pcOFF_pumpOFF:
            if (relayStatus) {
                // non può essere il rele on ed il PC off
                if ( fModulo10Seconds ) LOG_ERROR("Relè OFF quando invece il PC è ON");
                startAlarmActions();
                fIdleStatus=false;
            } else {
                if (!fIdleStatus) {
                    fIdleStatus=true;
                    resetAlarmActions(fIdleStatus);
                }
            }
            break;


        // non può essere la pompa ON ed il PC off
        case pcOFF_pumpON:
            if ( fModulo10Seconds ) LOG_ERROR("Pump ON quando il PC è OFF.");
            startAlarmActions();
            fIdleStatus=false;
            break;

        // status normale in attesa che si accenda la pompa
        case pcON_pumpOFF:
            if ( fModulo5minutes )  sendStatusToTelegram();
            pressControlLED.on(); // accendiamo fisso il LED
            pumpLED.off();        // facciamoòp lampeggiare
            fIdleStatus=false;
            break;


        // status normale con la pompa accesa
        case pcON_pumpON:
            if ( fModulo5minutes )  sendStatusToTelegram();
            pressControlLED.on();
            pumpLED.on();
            fIdleStatus=false;
            break;

        default:
            break;

    } // endo of switch


}


