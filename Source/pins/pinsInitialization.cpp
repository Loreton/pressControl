/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 18.42.27
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// - project headers files
// ---------------------------------
#define _I_AM_PIN_INITIALIZATION_CPP_
#define LOG_LEVEL_0x
#include "@logMacros.h"
#include "@a_mainProject.h"
#include "@pin_Definitions.h"
#include "@pin_Prototypes.h"








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
    //====================================================
    //= set input pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
   // setup_deBouncedButton(startButton       ,startButton_pin       , "startButton"       , LOW);
   // setup_deBouncedButton2(startButton       ,15       , "startButton"       , LOW);
   // setup_deBouncedButton2(startButton2      ,16       , "startButton2"       , LOW);
   // setup_deBouncedButton(startButton3      ,18       , "startButton3"       , LOW);
   // setup_deBouncedButton2(startButton       ,15       , "startButton"       , LOW);
   // setup_deBouncedButton2(startButton2      ,16       , "startButton2"       , LOW);
   setup_deBouncedButton(startButton3       ,18       , "startButton3"       , LOW);
   setup_deBouncedButton(startButton4       ,20       , "startButton4"       , LOW);

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




