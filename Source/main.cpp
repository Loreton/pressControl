//
// updated by ...: Loreto Notarantonio
// Date .........: 29-06-2025 17.51.34
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define     LOG_LEVEL_0
#define     LOG_LEVEL_1
#include    "@globalVars.h"
#include    "@debouncedButton_Class.h"
#include    "@ln_time.h"

// ---------------------------------
// - project headers files
// ---------------------------------
#include "@a_mainProject.h"




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
    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-06 - rel_type: %d", ln_RELEASE_TYPE);

    // Serial.begin(115200);
    lnSERIAL.begin(115200);
    delay(1000);
    printf0_FN("%s\n", pressControlVersion);


    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    time_setup();

    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    // pinsInitialization();
    startButton.init("startButton", startButton_pin, LOW);
    Serial.printf("%s\n", startButton.pinID());

}


bool first_run=true;
void loop() {

    // -----------------------------------
    // ------ 1st run
    // -----------------------------------
    if (first_run) {
        first_run=false;
        printf0_NFN("processing started....\n");
    }

    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read(300)) {
        Serial.printf("[%s] Rilasciato!\n", startButton.pinID());
        startButton_action();
    }

    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);


}


