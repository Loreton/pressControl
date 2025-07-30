//
// updated by ...: Loreto Notarantonio
// Date .........: 30-07-2025 17.53.13
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
    a01_pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    a02_pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    a03_pnON_pumpON,     // rele esterno - Pressione lunga.
    a05_relayON_pcOFF_pumpOFF,      // solo relay interno  - ERROR.
    a06_relayON_pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    a07_relayON_pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    a08_relayON_pnON_pumpON,     // rele esterno - Pressione lunga.

} ;


// const PROGMEM char *alarmState[] = {"[EXT_RELAY] ALL_OFF", "[EXT_RELAY] ABNORMAL_pumpON", "[EXT_RELAY] pcON", "[EXT_RELAY] PC+PUMP ON", "[INT_RELAY] ALL_OFF", "[INT_RELAY] ABNORMAL_pumpON", "[INT_RELAY] pcON", "[INT_RELAY] PC+PUMP ON", };

bool    first_run=true;
uint8_t actionState=0;
uint8_t lastActionState=1;
uint32_t lastDisplayTime=0;

void loop() {
    char durationBUFFER[16];
    uint32_t duration;
    uint32_t now=millis();
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




    pumpState.pressingLevelNotification(pumpNotificationCB);
    /**
     * Leggi lo stato della pompa
     * Se è stato rilasciato
    */
    if (pumpState.released()) {
        pumpHandler(&pumpState);
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
    uint8_t pumpStatus  = pumpState.isPressed();
    uint8_t pcStatus    = pressControlState.isPressed();

    // actionState = (pressControlRelay.isActive() * 4) + (pressControlState.isPressed()*2) + (pumpState.isPressed()*1);
    // actionState = (relayStatus * 4) + (pcStatus*2) + (pumpStatus*1);
    actionState = (pcStatus*2) + (pumpStatus*1);

    if (actionState != lastActionState || (now - lastDisplayTime) > 15000) { // facciamo comunque il display ogni 15 secondi
        lastActionState=actionState;
        lastDisplayTime=now;

        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);

        switch (actionState) {  // azzeriamo il bit del relay
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
                    duration=PRESS_CONTROL_RELAY_MAX_TIME;
                    LOG_NOTIFY("Attivazione  %s per la durata di %s",
                                                pressControlRelay.pinID(),
                                                lnLog.timeStamp(durationBUFFER, sizeof(durationBUFFER), duration, true) );
                    pressControlRelay.startPulse(duration);       // accendiamo anche il relay interno in modo da far partire il pulseTime
                }
                pressControlLED.on();         // accendiamo fisso il LED
                pumpLED.off(); // facciamoòp lampeggiare
                break;


            case a03_pnON_pumpON:
                if (! relayStatus) {
                    duration=PRESS_CONTROL_RELAY_MAX_TIME;
                    LOG_NOTIFY("Attivazione  %s per la durata di %s",
                                                pressControlRelay.pinID(),
                                                lnLog.timeStamp(durationBUFFER, sizeof(durationBUFFER), duration, true) );
                    pressControlRelay.startPulse(duration);       // accendiamo anche il relay interno in modo da far partire il pulseTime
                }

                LOG_NOTIFY("Accensione fissa per %s ", pressControlLED.pinID());
                pressControlLED.on();
                LOG_NOTIFY("Accensione fissa per %s ", pumpLED.pinID());
                pumpLED.on();


            // case a05_relayON_pcOFF_pumpOFF:
            //     LOG_ERROR("Relay ON bat pressControl is OFF");
            //     // startAlarmActions();
            //     break;

            // case a06_relayON_pcOFF_pumpON:
            //     LOG_ERROR("Relay ON bat pressControl is OFF");
            //     // startAlarmActions();
            //     break;

            // case a07_relayON_pcON_pumpOFF:
            //     LOG_NOTIFY("Normal condition - pump is OFF");
            //     break;

            // case a08_relayON_pnON_pumpON:
            //     if (pumpState.maxLevelReached()) {
            //         LOG_ERROR("pump ON has reached MAX time");
            //     }
            //     else {
            //         LOG_INFO("Normal condition - pump is ON");
            //     }


            //     // pressControlLED.on();
            //     // pumpLED.on();
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
