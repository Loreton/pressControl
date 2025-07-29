//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 14.12.32
//




#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    "lnGlobalVars.h"
#include    "lnSerialRead.h" // waitFor...

// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"

// extern "C" void lwip_hook_ip6_input() {
    // Funzione hook vuota per risolvere il problema di linking
    // undefined reference to lwip_hook_ip6_input'
// }


size_t initialMemory;
size_t finalMemory;




#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    initialMemory = ESP.getFreeHeap();

    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-07 - rel_type: %d", ln_RELEASE_TYPE);

    Serial.begin(115200);
    delay(1000);
    lnLog.init();
    LOG_INFO("%s", pressControlVersion);


    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    lnTime.setup(); // Chiama il metodo setup della tua istanza di LnTime



    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();

    // ---------------- calcolo memoria
    finalMemory = ESP.getFreeHeap();
    LOG_TRACE("memoria (bytes): initial=%ld - final=%ld - occupied=%ld", initialMemory, finalMemory, (initialMemory - finalMemory)); // Stima RAM allocata
}



// Definisce i possibili tipi di condizioni
enum ActionState : uint8_t {
    a00_ALL_IS_DOWN = 0,      // tutto spento.
    a01_pcOFF_pumpON, // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    a02_pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    a03_pnON_pumpON,     // rele esterno - Pressione lunga.

} ;


// const PROGMEM char *alarmState[] = {"[EXT_RELAY] ALL_OFF", "[EXT_RELAY] ABNORMAL_pumpON", "[EXT_RELAY] pcON", "[EXT_RELAY] PC+PUMP ON", "[INT_RELAY] ALL_OFF", "[INT_RELAY] ABNORMAL_pumpON", "[INT_RELAY] pcON", "[INT_RELAY] PC+PUMP ON", };

bool first_run=true;
uint8_t actionState=0;
uint8_t lastActionState=1;

void loop() {
    char durationBUFFER[16];
    uint32_t duration;
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }


    /*
    if (lnTime.isMinuteOClock()) {
        finalMemory = ESP.getFreeHeap();
        LOG_TRACE("memoria (bytes): initial=%ld - final=%ld - occupied=%ld", initialMemory, finalMemory, (initialMemory - finalMemory)); // Stima RAM allocata
    } */



    // -----------------------------------
    // ------ lettura/refresh dei pin di output
    // -----------------------------------
    activeBuzzer.update();
    passiveBuzzer.update();
    pressControlLED.update();
    pumpLED.update();

    pressControlRelay.update();
    magnetoTermicoRelay.update();



    startButton.pressingLevelNotification(startButtonNotificationCB);
    /**
     * Leggi lo stato dello startButton
     * Se è stato rilasciato
    */
    if (startButton.released()) {
        startButtonHandler(startButton.currentPressLevel());
        startButton.reset();
    }




    pumpState.pressingLevelNotification(pumpPressedNotificationCB);
    /**
     * Leggi lo stato della pompa
     * Se è stato rilasciato
    */
    if (pumpState.released()) {
        pumpStateHandlerCB(&pumpState);
        pumpState.reset();
    }


    pressControlState.pressingLevelNotification(pressControlNotificationCB);
    /**
     * Leggi lo stato del pressControl
     * Se è stato rilasciato
    */
    if (pressControlState.released()) {
        pressControlState.reset();
    }


    // -----------------------------------
    // ------ Action
    // -----------------------------------
    uint8_t relayStatus = pressControlRelay.isActive();
    uint8_t pcStatus    = pressControlState.isPressed();
    uint8_t pumpStatus  = pumpState.isPressed();

    // actionState = (pressControlRelay.isActive() * 4) + (pressControlState.isPressed()*2) + (pumpState.isPressed()*1);
    // actionState = (relayStatus * 4) + (pcStatus*2) + (pumpStatus*1);
    actionState = (pcStatus*2) + (pumpStatus*1);

    if (actionState != lastActionState) {
        lastActionState=actionState;

        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);

        switch (actionState) {
            case a00_ALL_IS_DOWN:
                pressControlLED.blinking(1000, 3000);
                pumpLED.blinking(1000, 3000);
                activeBuzzer.reset();
                passiveBuzzer.noTone();
                pressControlRelay.off();       // spegniamo epr sicurezza il relay interno
                magnetoTermicoRelay.off();       // spegniamo anche il secondo relay
                break;


            case a01_pcOFF_pumpON:
                startAlarmActions();
                break;

            /**
             * Se il pressControl va on vuol dire che:
             *    1. è stato attivato il rele interno
             *    2. è stato attivato il magnetotermico esterno
             * allra:
             *    1, comunque il pressControlState è configurato con un tempo massimo di pressione di 30 minuti
             *    2, accendiamo, per sicurezza, il relè interno con il timer di 35 minuti
             *
            */
            case a02_pcON_pumpOFF:
                if (! relayStatus) {
                    duration=35*60*1000;
                    LOG_INFO("Attivazione  %s per la durata di %s",
                                                pressControlRelay.pinID(),
                                                lnLog.timeStamp(durationBUFFER, sizeof(durationBUFFER), duration, true) );
                    pressControlRelay.startPulse(duration);       // accendiamo anche il relay interno in modo da far partire il pulseTime
                }
                pressControlLED.on();         // accendiamo fisso il LED
                pumpLED.off(); // facciamoòp lampeggiare
                break;


            case a03_pnON_pumpON:
                if (! relayStatus) {
                    duration=35*60*1000;
                    LOG_INFO("Attivazione  %s per la durata di %s",
                                                pressControlRelay.pinID(),
                                                lnLog.timeStamp(durationBUFFER, sizeof(durationBUFFER), duration, true) );
                    pressControlRelay.startPulse(duration);       // accendiamo anche il relay interno in modo da far partire il pulseTime
                }

                LOG_INFO("Accensione fissa per %s ", pressControlLED.pinID());
                pressControlLED.on();
                LOG_INFO("Accensione fissa per %s ", pumpLED.pinID());
                pumpLED.on();

            // case a04_relayON_pcOFF_ALARM:
            //     startAlarmActions();
            //     break;

            // case a05_relayON_pcOFF_pumpON_ALARM:
            //     startAlarmActions();
            //     break;

            // case a06_relayON_pcON_pumpOFF_OK:
            //     pressControlLED.on();
            //     pumpLED.off();
            // // waitForEnter();
            //     break;

            // case a07_relayON_pcON_pumpON_OK:
            //     pressControlLED.on();
            //     pumpLED.on();
            //     if (pumpState.maxLevelReached()) {

            //     }
            //     break;

            default:
                break;
        } // endo of switch
        // waitForEnter();
    } // end if





    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);
}



void startAlarmActions() {
    pressControlRelay.off();       // accendiamo anche il relay interno in modo da far partire il pulseTime
    magnetoTermicoRelay.startPulse(5000); // non abbiamo armi. proviamo a chiidere il relay esterno togliendo alimentazione
    pumpLED.blinking(500, 500);
    pressControlLED.blinking(500, 500);
    activeBuzzer.blinking(500, 500);
}


// void resetAlarmActions() {
//     pressControlRelay.off();       // accendiamo anche il relay interno in modo da far partire il pulseTime
//     magnetoTermicoRelay.startPulse(5000); // non abbiamo armi. proviamo a chiidere il relay esterno togliendo alimentazione
//     pumpLED.blinking(500, 500);
//     pressControlLED.blinking(500, 500);
//     activeBuzzer.blinking(500, 500);
// }
