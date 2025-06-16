//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 18.44.20
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
// #include "@pin_Structures.h"
#include "@pin_Prototypes.h"

    // Struttura per mantenere lo stato di ogni pulsante.
    // struct deBouncedButton_s {
    //     int pin;                  // Il pin GPIO a cui è collegato il pulsante.
    //     const char* name;         // Nome del pulsante per identificazione (opzionale, utile per debugging).
    //     int pressedLogicLevel;    // Livello logico che indica il pulsante premuto (LOW o HIGH).
    //     bool lastButtonState;     // Ultima lettura RAW del pin.
    //     unsigned long lastDebounceTime; // Ultimo momento in cui il pin ha cambiato stato RAW.
    //     bool buttonPressed;       // Stato debounced: true se premuto, false se rilasciato.
    // };


// void setup_deBouncedButton2(deBouncedButton_s &btn, int pin, const char* name, int pressedLogicLevel);

// deBouncedButton_s startButton;
// deBouncedButton_s startButton2;

// deBouncedButton_t  inpPINs[5];
// deBouncedButton_t *startButton3  = &inpPINs[0]; // Dichiarazione della struttura per il nostro pulsante.


#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-06 - rel_type: %d", ln_RELEASE_TYPE);

    // Serial.begin(115200);
    lnSERIAL.begin(115200);
    delay(2000);

    printf0_NFN("%s\n", pressControlVersion);

    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();
    // printf1_NFN("turning led on\n");
    // setup_deBouncedButton2(startButton       ,15       , "startButton"       , LOW);
    // setup_deBouncedButton2(startButton2      ,16       , "startButton2"       , LOW);
    // setup_deBouncedButton(startButton3       ,18       , "startButton3"       , LOW);
    // setup_deBouncedButton(startButton4       ,20       , "startButton4"       , LOW);

    // printf0_NFN("%s: %d\n", startButton.name,  startButton.pin);
    // printf0_NFN("%s: %d\n", startButton2.name, startButton2.pin);
    printf0_NFN("%s: %d\n", startButton3->name, startButton3->pin);
    printf0_NFN("%s: %d\n", startButton4->name, startButton4->pin);
    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    // pinON(pressControlLED);
}


bool first_run=true;
void loop() {

    // -----------------------------------
    // ------ 1st run
    // -----------------------------------
    if (first_run) {
        first_run=false;
        printf0_NFN("starting loop....\n");
        // alignToMinute(); // allineamento al minuto ... non so se mi serve
        // digitalWrite(pressControlLED->pin, pressControlLED->OFF);
        // pinOFF(pressControlLED);
        // pinOFF(pumpLED);
    }




}


// #endif