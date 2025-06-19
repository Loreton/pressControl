//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 10.10.32
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <Preferences.h>
// #include <fauxmoESP.h>




#define __I_AM_MAIN_CPP__
// ---------------------------------
// - project headers files
// ---------------------------------
#define LOG_LEVEL_0
#define LOG_LEVEL_1
#define LOG_LEVEL_2x
#define LOG_LEVEL_3x
#define LOG_LEVEL_4x
#include "@logMacros.h"
#include "@a_mainProject.h"
#include "@pin_Definitions.h"
#include "@pin_Prototypes.h"


#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-06 - rel_type: %d", ln_RELEASE_TYPE);

    // Serial.begin(115200);
    lnSERIAL.begin(115200);
    delay(1000);


    printf0_NFN("%s\n", pressControlVersion);

    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();
    printf0_NFN("%s: %d\n", startButton.name, startButton.pin);

    // deBouncedButton_t startButton2; // Dichiarazione della struttura per il nostro pulsante.
    // deBouncedButton_t startButton3; // Dichiarazione della struttura per il nostro pulsante.
    // setup_deBouncedButton(startButton2,  15, "startButton2", LOW);
    // setup_deBouncedButton(&startButton3,  18, "startButton3", LOW);
    // printf0_NFN("%s: %d\n", startButton2.name, startButton2.pin);
    // printf0_NFN("%s: %d\n", startButton3.name, startButton3.pin);


    // -----------------------------------
    // ------ set Time
    // -----------------------------------
}


bool first_run=true;
void loop() {

    // -----------------------------------
    // ------ 1st run
    // -----------------------------------
    if (first_run) {
        first_run=false;
        printf0_NFN("processing started....\n");
        // alignToMinute(); // allineamento al minuto ... non so se mi serve
        // digitalWrite(pressControlLED->pin, pressControlLED->OFF);
        // pinOFF(pressControlLED);
        // pinOFF(pumpLED);
    }

    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (read_deBouncedButton(&startButton, 300)) {
        Serial.printf("[%s] Rilasciato!\n", startButton.name);
        startButton_action();
    }

    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);


}


// #endif