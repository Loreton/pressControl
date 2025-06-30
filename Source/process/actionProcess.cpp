//
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 15.41.28
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_99
#include "@logMacros.h"
// #include "@debouncedButton_sClass.h"

// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"



void startButton_action(void) {
    static bool relayState = false; // Stato attuale del relè (false = spento, true = acceso).


    // Toggle dello stato del relè.
    relayState = !relayState;
    if (relayState) {
        digitalWrite(pressControlRelay_pin, LOW); // Accendi il relè (adatta a HIGH se il tuo relè è "attivo alto").
        printf99_FN("  --> Relè ACCESO!\n");
    } else {
        digitalWrite(pressControlRelay_pin, HIGH); // Spegni il relè (adatta a LOW se il tuo relè è "attivo alto").
        printf99_FN("  --> Relè SPENTO!\n");
    }
}