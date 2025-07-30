/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-07-2025 17.56.37
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type


// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"




/* #########################################
ref: /home/loreto/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-gpio.h
   #define LOW               0x0
   #define HIGH              0x1

   //GPIO FUNCTIONS
   #define INPUT             0x01
   #define OUTPUT            0x03
   #define PULLUP            0x04
   #define INPUT_PULLUP      0x05
   #define PULLDOWN          0x08
   #define INPUT_PULLDOWN    0x09
   #define OPEN_DRAIN        0x10
   #define OUTPUT_OPEN_DRAIN 0x13
   #define ANALOG            0xC0
######################################### */





// extern ButtonLongPress_Struct startButton;
// extern ButtonLongPress_Struct pumpState;

// queste definizioni devo essere esterne alla funzione altrimenti le perdiamo wuando si esce dalla funzione
// queste definizioni devo essere esterne alla funzione altrimenti le perdiamo wuando si esce dalla funzione
const PROGMEM uint32_t START_BUTTON_THRESHOLDS[]         = {400, 4000, 5000, 6000};
const PROGMEM uint32_t PUMP_STATE_THRESHOLDS[]           = {400, 5000, 15000, 20000, 25000 };
const PROGMEM uint32_t PRESS_CONTROL_STATE_THRESHOLDS[]  = {400, PRESS_CONTROL_PIN_MAX_TIME }; // 30 minuti
// queste definizioni devo essere esterne alla funzione altrimenti le perdiamo wuando si esce dalla funzione
// queste definizioni devo essere esterne alla funzione altrimenti le perdiamo wuando si esce dalla funzione




void pinsInitialization(void) {
    const uint8_t NUM_START_BUTTON_THRESHOLDS           = sizeof(START_BUTTON_THRESHOLDS) / sizeof(START_BUTTON_THRESHOLDS[0]);
    const uint8_t NUM_PUMP_STATE_THRESHOLDS             = sizeof(PUMP_STATE_THRESHOLDS)   / sizeof(PUMP_STATE_THRESHOLDS[0]);
    const uint8_t NUM_PRESS_CONTROL_STATE_THRESHOLDS    = sizeof(PRESS_CONTROL_STATE_THRESHOLDS)   / sizeof(PRESS_CONTROL_STATE_THRESHOLDS[0]);

    //====================================================
    //= set input pins
    //====================================================
    // ------  name,                 pin_nr          active_level   );
    startButton.init("startButton", startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS); // Now an object, not a struct
    LOG_NOTIFY("\t[%s] initialized", startButton.pinID());

    pumpState.init("pumpState", pumpState_pin, LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS);   // Now an object, not a struct
    LOG_NOTIFY("\t[%s] initialized", pumpState.pinID());

    pressControlState.init("pressControlState",  pressControlState_pin,  LOW, PRESS_CONTROL_STATE_THRESHOLDS, NUM_PRESS_CONTROL_STATE_THRESHOLDS);   // Now an object, not a struct
    LOG_NOTIFY("\t[%s] initialized", pressControlState.pinID());

    #if LOG_LEVEL >= LOG_LEVEL_TRACE
        startButton.showStatus();
        pumpState.showStatus();
        pressControlState.showStatus();
    #endif


    //====================================================
    //= set output pins
    //====================================================
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    LOG_NOTIFY("\t[%s] initialized", activeBuzzer.pinID());

    pressControlLED.init("pressControlLED", pressControlLED_pin, HIGH);
    LOG_NOTIFY("\t[%s] initialized", pressControlLED.pinID());

    pumpLED.init("pumpLED", pumpLED_pin, HIGH);
    LOG_NOTIFY("\t[%s] initialized", pumpLED.pinID());

    passiveBuzzer.init("passiveBuzzer", passiveBuzzer_pin, HIGH, 0, 10);
    LOG_NOTIFY("\t[%s] initialized", passiveBuzzer.pinID());

    pressControlRelay.init("pressControlRelay", pressControlRelay_pin, LOW);
    LOG_NOTIFY("\t[%s] initialized", pressControlRelay.pinID());

    magnetoTermicoRelay.init("magnetoTermicoRelay", magnetoTermicoRelay_pin, LOW);
    LOG_NOTIFY("\t[%s] initialized", magnetoTermicoRelay.pinID());


}




