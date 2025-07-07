//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 16.54.56
//




#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    "lnLogger.h"
#include    "lnTime.h"
#include    "callBackPrototypes.h"
#include    "ButtonLongPress_Struct.h"

// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"

extern "C" void lwip_hook_ip6_input() {
    // Funzione hook vuota per risolvere il problema di linking
    // undefined reference to lwip_hook_ip6_input'
}


size_t initialMemory;
size_t finalMemory;

// --- creazione oggetti
// ButtonLongPress_Struct startButton;




#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-06 - rel_type: %d", ln_RELEASE_TYPE);

    Serial.begin(115200);
    delay(1000);
    initialMemory = ESP.getFreeHeap();
    LOG_INFO("%s", pressControlVersion);

    /*
        // calcolo memoria
        ButtonDebounced_Class* obj = new ButtonDebounced_Class();
        size_t before = ESP.getFreeHeap();
        size_t after = ESP.getFreeHeap();
        Serial.println(before - after); // Stima RAM allocata
    */
    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    time_setup();

    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();

    LOG_NOTIFY("%s", startButton.pinID());

    finalMemory = ESP.getFreeHeap();
    LOG_DEBUG("initial Memory:     %ld bytes", initialMemory); // Stima RAM allocata
    LOG_DEBUG("final   Memory:     %ld bytes", finalMemory); // Stima RAM allocata
    LOG_DEBUG("memoria occupata:   %ld bytes", finalMemory - initialMemory); // Stima RAM allocata
}





bool first_run=true;
void loop() {
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }

    // -----------------------------------
    // ------ refresh dei pin
    // -----------------------------------
    activeBuzzer.updateStatus();
    pressControlLED.updateStatus();
    pumpLED.updateStatus();
    startButton.notifyCurrentButtonLevel(beepNotification);
    pumpState.notifyCurrentButtonLevel(beepNotification);



    #ifdef USE_ACTION_CALL_BACK
        startButton.read(startButtonHandlerCB);
        pumpState.read(pumpStateHandlerCB);
    #else
        // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
        if (startButton.read()) {
            // LOG_NOTIFY("[%s] Rilasciato!", startButton.pinID());
            startButtonHandlerCB(&startButton);
            // startButton_action();
        }

        // Leggi lo stato della pompa. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
        if (pumpState.read()) {
            // LOG_NOTIFY("[%s] Rilasciato!", pumpState.pinID());
            pumpStateHandlerCB(&pumpState);
            // startButton_action();
        }
    #endif

    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    // delay(10);




}


