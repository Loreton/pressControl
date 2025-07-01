//
// updated by ...: Loreto Notarantonio
// Date .........: 01-07-2025 18.20.41
//
/**
ESP32 - devo controllare una pompa provvista di un press-control.
    Ho un relè per comandare il press-control.

    un pin per monitorare status del press-control
    un pin per monitorare status della pompa

    un led per mostrare status del press-control
    un pin per monitorare status della pompa

    un buzzer passivo per segnalare con suoni
    un buzzer attivo per inviare beep

    un pulsante per accendere e spegnere il press-control

    Una volta che viene rilevata l'acennsione della pompa emettere un suono di 4 frequenze a salire sul buzzer passivo ed a scalare nel caso la pompa si spegne.

    Se la pompa continua a rimanere accesa allora emettere un beep dopo 60 secondi, un beep dopo altri 55 secondi, un beep dopo altri 50 secondi e così via per 6 step.

    Dopo l'ultimo step:
    1. spegnere il press-control
    2. se il press-control o la pompa dovessero essere ancora accesi emettere un beep ogni 5 sec.

    Comunque il press-control impostare un timer per spegnerlo dopo 30 minuti

    Lo stato del press-control, della pompa  e del tempo rimasto del timer devono essere inviati a telegram




    Esp32 ho un relè che comanda una pompa ed un pin che controlla il suo stato.  Una volta accesa emettere in beep dopo  60 secondi,
    un beep dopo altri 55 secondi, un beep dopo altri 50 secondi e così via per 6 step. Dopo l'ultimo step emettere un beep ogni 5 sec.
*/



#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define     LOG_LEVEL_0
#define     LOG_LEVEL_1
#include    "@globalVars.h"
#include    "@ln_time.h"

// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"


size_t initialMemory;
size_t finalMemory;

// --- creazione oggetti
// ButtonDebounced_Class startButton;




#ifdef __LN_TIME_INCLUDED__   // definito in ln_time.h
    extern struct tm timeinfo;  // capire se va bene uno per tutti i moduli oppure mantenerli separati per evitare overwrites

#else
    // #########################################
    // # se non c'è ln_time.cpp mi serve una dummy_Now()
    // #########################################
    const int8_t DUMMY_TIME_BUFFER_LENGTH = 20;
    char  PROGMEM temp_buffer_time[DUMMY_TIME_BUFFER_LENGTH];
    char *nowTime() {
        snprintf(temp_buffer_time, DUMMY_TIME_BUFFER_LENGTH, "%s", "01:02:03");
        return temp_buffer_time;
    }
#endif


#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    initialMemory = ESP.getFreeHeap();
    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-06 - rel_type: %d", ln_RELEASE_TYPE);

    // Serial.begin(115200);
    lnSERIAL.begin(115200);
    delay(1000);
    printf0_FN("%s\n", pressControlVersion);

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
    // startButton.init("startButton", startButton_pin, LOW,           START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS); // Now an object, not a struct
    // pumpState.init("pumpState",     pumpState_pin,   LOW,           PUMP_STATE_THRESHOLDS,   NUM_PUMP_STATE_THRESHOLDS);   // Now an object, not a struct




    //====================================================
    //= set output pins
    //====================================================
    // activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    // pressControlLED.init("pressControlLED", pressControlLED_pin, HIGH);
    // pumpLED.init("pumpLED", pumpLED_pin, HIGH);

    Serial.printf("%s\n", startButton.pinID());

    finalMemory = ESP.getFreeHeap();
    printf0_FN("initial Memory:     %ld bytes\n", initialMemory); // Stima RAM allocata
    printf0_FN("final   Memory:     %ld bytes\n", finalMemory); // Stima RAM allocata
    printf0_FN("memoria occupata:   %ld bytes\n", finalMemory - initialMemory); // Stima RAM allocata
}


bool first_run=true;



void loop() {
    if (first_run) {
        first_run=false;
        printf0_NFN("processing started....\n");
    }

    // -----------------------------------
    // ------ refresh dei pin
    // -----------------------------------
    activeBuzzer.updateStatus();
    pressControlLED.updateStatus();
    pumpLED.updateStatus();
    startButton.notifyCurrentLevel(&activeBuzzer);
    pumpState.notifyCurrentLevel(&activeBuzzer);


    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read()) {
        Serial.printf("[%s] Rilasciato!\n", startButton.pinID());
        startButton_action();
    }

    // Leggi lo stato della pompa. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (pumpState.read()) {
        Serial.printf("[%s] Rilasciato!\n", pumpState.pinID());
        // startButton_action();
    }

    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);


}


