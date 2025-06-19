//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 07.59.48
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
// #include "@pin_Prototypes.h"



void startButton_action(void) {
    static bool relayState = false; // Stato attuale del relè (false = spento, true = acceso).


    // Toggle dello stato del relè.
    relayState = !relayState;
    if (relayState) {
        digitalWrite(pressControlRelay_pin, LOW); // Accendi il relè (adatta a HIGH se il tuo relè è "attivo alto").
        Serial.println("  --> Relè ACCESO!");
    } else {
        digitalWrite(pressControlRelay_pin, HIGH); // Spegni il relè (adatta a LOW se il tuo relè è "attivo alto").
        Serial.println("  --> Relè SPENTO!");
    }
}