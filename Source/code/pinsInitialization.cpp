/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 17.47.53
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
// #include "@lnLibrary.h"
#include "@mainStructures.h"
#include "@pinOperations.h"




// ---------------------------------
// - project headers files
// ---------------------------------
#include "@pinDefinitions.h"
#include "@prjStructures.h"





/*
    valori di soglia per pressione tasti
*/

        const char * PROGMEM THRESHOLD_LEVEL_TYPES[] = {"NO_PRESSED_BUTTON", "PRESSED_LEVEL_01", "PRESSED_LEVEL_02", "PRESSED_LEVEL_03", "PRESSED_LEVEL_04", "PRESSED_LEVEL_05", "PRESSED_LEVEL_06", "OVERFLOW_TIME"};
        const int8_t THRESHOLDS_LEVELS_TYPES_length = sizeof(THRESHOLD_LEVEL_TYPES)/sizeof(char *);


#define END_OF_ARRAY -1 // --- thresholds value (terminated by -1 as last element)
#ifdef PRODUCTION
    const int32_t PROGMEM pumpState_thresholds_level_values[]   = {0, 100, 1*60*1000,  2*60*1000,  3*60*1000,  4*60*1000,  5*60*1000,  END_OF_ARRAY};
#else
    const int32_t PROGMEM pumpState_thresholds_level_values[]   = {0, 100, 10*1000,   20*1000,     30*1000,     40*1000,     50*1000,  END_OF_ARRAY};
#endif

const int32_t PROGMEM base_thresholds_level_values[]        = {0, 100, 1000, END_OF_ARRAY};
const int32_t PROGMEM startButton_thresholds_level_values[] = {0, 1000, 2000, END_OF_ARRAY};




const int8_t INPUT_PINS  = 3;
io_input_pin_struct_t inpPINs[INPUT_PINS+1];
io_input_pin_struct_t *pumpState           = &inpPINs[0]; // input
io_input_pin_struct_t *startButton         = &inpPINs[1]; // input
io_input_pin_struct_t *pressControlState   = &inpPINs[2]; // input


const int8_t OUTPUT_PINS = 6;
io_output_pin_struct_t outPINs[OUTPUT_PINS+1];
io_output_pin_struct_t *pressControlRelay   = &outPINs[0];
// io_output_pin_struct_t *loadSuperioreRelay  = &outPINs[1];
io_output_pin_struct_t *activeBuzzer        = &outPINs[2];
io_output_pin_struct_t *passiveBuzzer       = &outPINs[3];
io_output_pin_struct_t *pumpLED             = &outPINs[4];
io_output_pin_struct_t *pressControlLED     = &outPINs[5];


// io_output_pin_struct_t *LED                = &outPINs[4];
// io_output_pin_struct_t *led_internal       = &outPINs[5];
/*
    #########################################
    # array ending with negative number
    # iint16_t array[] = {400, 1500, 3000, 4000, -1};
    #########################################
*/
int8_t intArrayLength(const int32_t *p) {
    uint8_t lun=0;
    // while (*p >= 0) {
    while (*p != END_OF_ARRAY) {
        lun++;
        p++;
    }
    return lun;
}

/*
    #########################################
    # initializzazione pin
    #########################################
*/
void inputPinInit(const char *name, uint8_t nr, io_input_pin_struct_t *p, uint8_t mode, bool active_level, const int32_t *thresholds) {
    // --- non verranno più modificati
    p->name           = name;
    p->pin            = nr;
    p->mode           = mode;
    p->active_level   = active_level;
    p->OFF            = !active_level;
    p->ON             = active_level;
    snprintf(p->pinID, 20, "[%02d.%-14s]:", p->pin, p->name);

    p->thresholds     = thresholds;
    p->n_thresholds = intArrayLength(thresholds); // --- calculate thresholds array length

    //  verifica per sicurezza
    if (p->n_thresholds > THRESHOLDS_LEVELS_TYPES_length) {
        printf1_NFN("p->n_thresholds (%d) > THRESHOLDS_LEVELS_TYPES_length (%d)... changing to: (%d)", p->n_thresholds, THRESHOLDS_LEVELS_TYPES_length, THRESHOLDS_LEVELS_TYPES_length);
        p->n_thresholds = THRESHOLDS_LEVELS_TYPES_length; // per sicurezza
    }


    pinMode(p->pin, p->mode);

    readInputPin(p);

    delay(50);

}


/*
    #########################################
    # initializzazione pin
    #########################################
*/
void outputPinInit(const char *name, uint8_t nr, io_output_pin_struct_t *p, uint8_t mode, bool active_level) {
    // --- non verranno più modificati
    p->name           = name;
    p->pin            = nr;
    p->mode           = mode;
    p->active_level   = active_level;
    p->OFF            = !active_level;
    p->ON             = active_level;
    snprintf(p->pinID, 20, "[%02d.%-14s]:", p->pin, p->name);

    // --- process pin MODE
    if ( (p->mode == OUTPUT) || (p->mode == OUTPUT_OPEN_DRAIN) ) {
        digitalWrite(p->pin, p->OFF);
        delay(50);
    }

    pinMode(p->pin, p->mode);

    delay(50);
}







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
    inputPinInit("Pump"              , pumpState_pin         , pumpState         , INPUT_PULLUP , LOW , &pumpState_thresholds_level_values[0]);
    inputPinInit("PressControlState" , pressControlState_pin , pressControlState , INPUT_PULLUP , LOW , &base_thresholds_level_values[0]);
    inputPinInit("startButton"       , startButton_pin       , startButton       , INPUT_PULLUP , LOW , &startButton_thresholds_level_values[0]);



    //====================================================
    //= set output pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
    outputPinInit("autoclave"         , pressControlRelay_pin , pressControlRelay  , OUTPUT , LOW);
    // outputPinInit("loadSuperiore"     , loadSuperiore_pin     , loadSuperioreRelay , OUTPUT , LOW);
    // outputPinInit("LED"               , LED_pin               , LED                , OUTPUT , LOW);
    // outputPinInit("led_internal_pin"  , led_internal_pin      , led_internal       , OUTPUT , LOW);
    outputPinInit("activeBuzzer"      , activeBuzzer_pin      , activeBuzzer       , OUTPUT , HIGH);
    outputPinInit("passiveBuzzer"     , passiveBuzzer_pin     , passiveBuzzer      , OUTPUT , HIGH);
    outputPinInit("pumpLED"           , pumpLED_pin           , pumpLED            , OUTPUT , HIGH);
    outputPinInit("pressControlLED"   , pressControlLED_pin   , pressControlLED    , OUTPUT , HIGH);

    pressControlRelay->pulsetime.msecs_default=30*60*1000;  // seconds default pulsetime per PressControl = 30 minuti
    // loadSuperioreRelay->pulsetime.msecs_default=5*60*1000; // seconds default pulsetime per loadSuperiore = 5 minuti
}





