/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 19.59.51
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#define LOG_LEVEL_1x
#include "@logMacros.h"


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
void pinsInitialization(void) {
    pinMode(pressControlRelay_pin, OUTPUT);
    //====================================================
    //= set input pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
    startButton.init("startButton", startButton_pin, LOW);




    //====================================================
    //= set output pins
    //====================================================
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    pressControlLED.init("pressControlLED", pressControlLED_pin, HIGH);
    pumpLED.init("pumpLED", pumpLED_pin, HIGH);

}




