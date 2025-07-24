//
// updated by ...: Loreto Notarantonio
// Date .........: 24-07-2025 20.16.34
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



// -----------------------------------
// ------ calcolo parametri
// -----------------------------------
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
// const PROGMEM char *alarmState[] = {
//                             "RELAY OFF - PC OFF - PUMP OFF  - [OK]",
//                             "RELAY OFF - PC OFF - PUMP ON   - [ERROR]",
//                             "RELAY OFF - PC ON  - PUMP OFF  - [OK - ext relay]",
//                             "RELAY OFF - PC ON  - PUMP ON   - [OK - ext relay]",
//                             "RELAY ON  - PC OFF - PUMP OFF  - [ERROR]",
//                             "RELAY ON  - PC OFF - PUMP ON   - [ERROR]",
//                             "RELAY ON  - PC ON  - PUMP OFF  - [OK]",
//                             "RELAY ON  - PC ON  - PUMP ON   - [OK]",
//                                 };

// Definisce i possibili tipi di pressione del pulsante.
enum ActionState : uint8_t {
    // relay interno spento, quindi si presume che sia attivo quello esterno
    // è comunque una situazione temporanea perché non appena si accente il pressContro, attiviamo anche il relayInterno
    ALL_IS_DOWN = 0,      // tutto spento.
    ALARM_PC_OFF_PUMP_ON, // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    OK_PC_ON_PUMP_OFF,    // rele esterno - PressControl ON (con il rele esterno)
    OK_PC_ON_PUMP_ON,     // rele esterno - Pressione lunga.

    // relay interno acceso
    ALARM_RELAY_ON_PC_OFF,         // acceso ma non è partito il pressControl . should not occur
    ALARM_RELAY_ON_PC_OFF_PUMP_ON, // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    OK_RELAY_ON_PC_ON_PUMP_OFF,    // PressControl ON e Pump OFF
    OK_RELAY_ON_PC_ON_PUMP_ON,     // Pressione lunga.
} ;


// const PROGMEM char *alarmState[] = {"[EXT_RELAY] ALL_OFF", "[EXT_RELAY] ABNORMAL_PUMP_ON", "[EXT_RELAY] PC_ON", "[EXT_RELAY] PC+PUMP ON", "[INT_RELAY] ALL_OFF", "[INT_RELAY] ABNORMAL_PUMP_ON", "[INT_RELAY] PC_ON", "[INT_RELAY] PC+PUMP ON", };

bool first_run=true;
uint8_t actionState=0;
uint8_t lastActionState=1;

void loop() {
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }


    // -----------------------------------
    // ------ lettura/refresh dei pin
    // -----------------------------------
    activeBuzzer.update();
    passiveBuzzer.update();
    pressControlLED.update();
    pumpLED.update();

    pressControlRelay.update();
    magnetoTermicoRelay.update();

    startButton.pressingLevelNotification(startButtonNotificationCB);
    pumpState.pressingLevelNotification(pumpPressedNotificationCB);
    pressControlState.pressingLevelNotification(pressControlNotificationCB);




    // -----------------------------------
    // ------ Action
    // -----------------------------------
    uint8_t relayStatus = pressControlRelay.isActive();
    uint8_t pcStatus    = pressControlState.isPressed();
    uint8_t pumpStatus  = pumpState.isPressed();

    // actionState = (pressControlRelay.isActive() * 4) + (pressControlState.isPressed()*2) + (pumpState.isPressed()*1);
    actionState = (relayStatus * 4) + (pcStatus*2) + (pumpStatus*1);

    if (actionState != lastActionState) {
        lastActionState=actionState;

        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);

        switch (actionState) {
            case ALL_IS_DOWN:
                pumpLED.blinking(1000, 3000);
                pressControlLED.blinking(1000, 3000);
                activeBuzzer.reset();
                passiveBuzzer.noTone();
                break;

            case ALARM_PC_OFF_PUMP_ON:
                startAlarmActions();
                break;

            case OK_PC_ON_PUMP_OFF:
                if (! relayStatus) {
                    pressControlRelay.startPulse(30*60);       // accendiamo anche il relay interno in modo da far partire il pulseTime
                }
                pressControlLED.on();         // accendiamo fisso il LED
                pumpLED.blinking(2000, 1000); // facciamoòp lampeggiare
                break;

            case OK_PC_ON_PUMP_ON:
                if (! relayStatus) {
                    pressControlRelay.startPulse(30*60);       // accendiamo anche il relay interno in modo da far partire il pulseTime
                }
                pressControlLED.on();
                pumpLED.on();

            case ALARM_RELAY_ON_PC_OFF:
                startAlarmActions();
                break;

            case ALARM_RELAY_ON_PC_OFF_PUMP_ON:
                startAlarmActions();
                break;

            case OK_RELAY_ON_PC_ON_PUMP_OFF:
                pressControlLED.on();
                pumpLED.off();
                break;

            case OK_RELAY_ON_PC_ON_PUMP_ON:
                pressControlLED.on();
                pumpLED.on();
                if (pumpState.isMaxLevelReached()) {

                }
                break;

            default:
                break;
        }

    }



    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read()) {
        startButtonHandler(startButton.pressedLevel());
        startButton.reset();
    }



    // Leggi lo stato della pompa. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (pumpState.read()) {
        pumpStateHandlerCB(&pumpState);
        pumpState.reset();
    }


    /**
     * Leggi lo stato del pressControl
     * Se è stato rilasciato
    */
    if (pressControlState.read()) {
        pressControlState.reset();
    }

    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(100);

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
