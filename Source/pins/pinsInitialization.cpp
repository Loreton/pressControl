/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 16.24.45
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#define LOG_LEVEL_1x
#include "@logMacros.h"
// #include "@buttonDebounced_Class.h"

// extern DebouncedButton_Class startButton; // Dichiarazione della struttura function inside.
// ---------------------------------
// - project headers files
// ---------------------------------
// #define _I_AM_PIN_INITIALIZATION_CPP_
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
void pinsInitialization(void) {
   pinMode(pressControlRelay_pin, OUTPUT);
    //====================================================
    //= set input pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
   startButton.init("startButton", startButton_pin, LOW);
   activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
   pressControlLED.init("pressControlLED", pressControlLED_pin, HIGH);
   pumpLED.init("pumpLED", pumpLED_pin, HIGH);




    //====================================================
    //= set output pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
    // uint8_t specialOutputLevel;

    // outputPinInit(alexaName           , pressControlRelay_pin , pressControlRelay  , OUTPUT , SPECIAL_ON_OFF_LEVEL);
    // outputPinInit("pumpHornAlarm"     , pumpHornAlarm_pin     , pumpHornAlarmRelay , OUTPUT , SPECIAL_ON_OFF_LEVEL);

    // outputPinInit("activeBuzzer"      , activeBuzzer_pin      , activeBuzzer       , OUTPUT , HIGH);
    // outputPinInit("passiveBuzzer"     , passiveBuzzer_pin     , passiveBuzzer      , OUTPUT , HIGH);
    // outputPinInit("pumpLED"           , pumpLED_pin           , pumpLED            , OUTPUT , HIGH);
    // outputPinInit("pressControlLED"   , pressControlLED_pin   , pressControlLED    , OUTPUT , HIGH);

    // pressControlRelay->pulsetime.msecs_default=30*60*1000;  // seconds default pulsetime per PressControl = 30 minuti
}




