//
// updated by ...: Loreto Notarantonio
// Date .........: 11-09-2025 18.03.30
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
uint32_t pressControl_remaining;
uint32_t pump_remaining;
uint32_t relay_remainig;
const bool fForce = true;

// Definisce i possibili tipi di condizioni
enum ActionState : uint8_t {
    pcOFF_pumpOFF = 0,      // tutto spento.
    pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    pressControlON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    pressControlON_pumpON,     // rele esterno - Pressione lunga.
    PUMP_ALARM,
} ;



void readDevicesStatus(void) {
    relayStatus            = pressControlRelay.isActive();
    pumpStatus             = pumpState.isPressed();
    pcStatus               = pressControl.isPressed();

    pressControl_remaining = pressControl.timeToMaxThresholdLevel();
    pump_remaining         = pumpState.timeToMaxThresholdLevel();
    relay_remainig         = pressControlRelay.remainingPulseTime();

}


// #############################################################
// #
// #############################################################
void telegramSendDevicesStatus(void) {
    readDevicesStatus();

    setTelegramTitle();

    myBot.addFormattedString("<b>Relay:</b> %s\n", str_OnOff[relayStatus]);
    if (relayStatus && relay_remainig != 0) {
        myBot.addFormattedString("\t<i>remainig: %s</i>\n", lnTime.msecToHMS(relay_remainig));
    }

    myBot.addFormattedString("<b>PressControl:</b> %s\n", str_OnOff[pcStatus]);
    if (pcStatus && pressControl_remaining != 0) {
        myBot.addFormattedString("\t<i>remainig: %s</i>\n", lnTime.msecToHMS(pressControl_remaining));
    }

    myBot.addFormattedString("<b>PUMP:</b> %s\n", str_OnOff[pumpStatus]);
    if (pumpStatus && pump_remaining != 0) {
        myBot.addFormattedString("\t<i>remainig: %s</i>\n", lnTime.msecToHMS(pump_remaining));
    }

    myBot.send();
}



// #############################################################
// #
// #############################################################
void startAlarmActions() {
    static uint32_t relay_delay=MAGNETOTERMIC_RELAY_PULSETIME + 3000;
    static uint32_t last_relay_time;
    // LOG_SPEC("DUMP trap......");

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
        LOG_INFO("Synching LED blinking"); // NO perchè compare ad ogni giro di loop
        pressControlLED.reset(); // per sincronizzare i led
        pumpLED.reset(); // per sincronizzare i led
        activeBuzzer.blinking(100, 100, 3);
        activeBuzzer.waitForPulseEnding(1000);

    }
    pressControlLED.blinking(1000, 3000);
    pumpLED.blinking(1000, 3000);
}



// #############################################################
// #
// #############################################################
void chackActionStatus() {
    static bool fIdleStatus = false; // flag per indicare situazione di tutto spento

    static  uint8_t  lastActionState = 1;
            uint32_t now = millis();

            bool    actionStateChanged;
            uint8_t actionState;

    // -----------------------------------
    // ------ Action
    // -----------------------------------
    readDevicesStatus();

    // relayStatus = pressControlRelay.isActive();
    // pumpStatus  = pumpState.isPressed();
    // pcStatus    = pressControl.isPressed();

    // pressControl_remaining = pressControl.timeToMaxThresholdLevel();
    // pump_remaining         = pumpState.timeToMaxThresholdLevel();
    // relay_remainig         = pressControlRelay.remainingPulseTime();


    // azzeriamo l'allarme se la pompa è spenta
    if (!pcStatus) {fPUMP_ALARM = false; }

    if (fPUMP_ALARM) {
        actionState = PUMP_ALARM;
        LOG_ERROR("Pump Alarm status: %d", fPUMP_ALARM);
    } else {
        actionState = (pcStatus*2) + (pumpStatus*1);
    }

    actionStateChanged = (actionState == lastActionState) ? false : true;

    bool forceSend = actionStateChanged ? true : false;

    // if ( actionStateChanged || modulo_05_minutes ) { // facciamo comunque il display ogni 15 secondi
    if ( actionStateChanged  ) {
        lastActionState=actionState;
        // LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        // LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);
        // LOG_INFO("actionState [%02d]: %12s", actionState, lnTime.msecToHMS(pressControl_remaining);


        LOG_INFO("actionState [%02d]:", actionState);
        LOG_INFO("   RELAY:         %-7s - remaining: %s", str_OnOff[relayStatus],    lnTime.msecToHMS(relay_remainig));
        LOG_INFO("   PRESS-CONTROL: %-7s - remaining: %s", str_OnOff[pcStatus],       lnTime.msecToHMS(pressControl_remaining));
        LOG_INFO("   PUMP:          %-7s - remaining: %s", str_OnOff[pumpStatus],     lnTime.msecToHMS(pump_remaining));

        LOG_INFO("invio dello status su Telegram");
        telegramSendDevicesStatus();
    }





    switch (actionState) {

        // status normale in attesa che si accenda il PC
        case PUMP_ALARM:
            // LOG_SPEC("DUMP trap......");
            if ( modulo_10_seconds ) {
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
            // LOG_SPEC("DUMP trap......");

            if (relayStatus) {
                // non può essere il rele on ed il PC off
                if ( modulo_10_seconds ) LOG_ERROR("Relè OFF quando invece il PC è ON");
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
            // LOG_SPEC("DUMP trap......");
            if ( modulo_10_seconds ) LOG_ERROR("Pump ON quando il PC è OFF.");
            startAlarmActions();
            fIdleStatus=false;
            break;

        // status normale in attesa che si accenda la pompa
        case pressControlON_pumpOFF:
            if ( modulo_05_minutes ) {
                LOG_INFO("invio dello status su Telegram");
                telegramSendDevicesStatus();
            }
            pressControlLED.on(); // accendiamo fisso il LED
            pumpLED.off();        // facciamoòp lampeggiare
            fIdleStatus=false;
            break;


        // status normale con la pompa accesa
        case pressControlON_pumpON:
            if ( modulo_02_minutes ) {
                LOG_INFO("invio dello status su Telegram");
                telegramSendDevicesStatus();
            }
            pressControlLED.on();
            pumpLED.on();
            fIdleStatus=false;
            break;

        default:
            break;

    } // endo of switch


}


