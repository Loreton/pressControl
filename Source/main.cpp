//
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 20.28.43
//




#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    "lnGlobalVars.h"
#include    "lnSerialRead.h"
#include    "lnLogger.h"
#include    "lnTime.h"
#include    "callBackPrototypes.h"
#include    "ButtonLongPress_Struct.h"
#include    "PassiveBuzzer_Struct.h"

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




// #define VERSION_LENGTH 40
// char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    // snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-07 - rel_type: %d", ln_RELEASE_TYPE);

    Serial.begin(115200);
    delay(1000);
    myLog.begin();
    initialMemory = ESP.getFreeHeap();
    // LOG_INFO("%s", pressControlVersion);



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
    // ------ lettura/refresh dei pin
    // -----------------------------------
    activeBuzzer.update();
    pressControlLED.update();
    pumpLED.update();

    pressControlRelay.update();
    magnetoTermicoRelay.update();

    startButton.notifyPressingLevel(beepNotification);
    pumpState.notifyPressingLevel(beepNotification);
    pressControlState.notifyPressingLevel(beepNotification);

    /***
        tabella decisionale
        ; situazione in cui l'accenzione è comandata dal magnetotermico direttamente

        relay=4    pressControl=2    pump=1 |  value  action
            0           0               0   |    0       normal - pumpLED & pressControlLED lighting
            0           0               1   |    1       abnormal - Allarme continuo
            0           1               0   |    2       pressControlLED ON, pumpLED OFF
            0           1               1   |    3       pressControlLED ON, pumpLED ON
                                                            qui si calcola il tempo in cui la pompa rimane accesa
                                                            sono diverse fasi. per ogni fase viene emesso un beeep e dopo
                                                            l'ultima fase il beep è continuo e si prevede di spegnere forzatamente la pompa

                                                            - azione sul rele2
                                                            accendendo (per un paio di secondi) il secondo relè, che dà tensione al magnetotermico,
                                                            si toglie alimentazione al magnetotermico e il presscontrol/pompa dovrebbero spegnersi.
                                                            e si dovrebbe tornare alla posizione 0.

        ; situazione in cui l'accenzione è comandata dal relè interno
        relay=4    pressControl=2    pump=1 |  value  action
            1           0               0   |    4       normal - pumpLED & pressControlLED lighting
            1           0               1   |    5       abnormal - Allarme continuo
            1           1               0   |    6       pressControlLED ON, pumpLED OFF
            1           1               1   |    7       pressControlLED ON, pumpLED ON
                                                            qui si calcola il tempo in cui la pompa rimane accesa
                                                            sono diverse fasi. per ogni fase viene emesso un beeep e dopo
                                                            l'ultima fase il beep è continuo e si prevede di spegnere forzatamente la pompa
                                                            Spegnere il relay1, se non si spegne il presscontrol
                                                            attivare il protocollo 'azione sul rele2'

    ***/

    uint8_t state = (pressControlRelay.isActive() * 4) + (pressControlState.isPressed()*2) + (pumpState.isPressed()*1);
    LOG_INFO("current state value: %d", state);

    switch (state) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            break;

        default:
            break;
    }

    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read()) {
        startButtonHandler(&startButton);
    }



    // Leggi lo stato della pompa. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (pumpState.read()) {
        pumpStateHandlerCB(&pumpState);
    }

    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(100);

}


