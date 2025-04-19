//
// updated by ...: Loreto Notarantonio
// Date .........: 19-04-2025 14.51.55
//

#include <Arduino.h>

#ifndef __PROJECT_STRUCTURES_H__
    #define __PROJECT_STRUCTURES_H__
    #include "@mainStructures.h"


    enum ErrorCode   : uint8_t { ERROR_01 = 1, ERROR_10 = 10 };
    enum outo_on     : bool { fAUTO_ON_OFF = true, fNO_AUTO_ON_OFF = false};

    enum displayData : uint8_t { NO_DISPLAY=0, DISPLAY_STATUS}; // come parametro per fare il display in alcune funzioni (es.: is_acted())

    enum caller      : uint8_t { NO_TELEGRAM = 0,
                                CALLED_BY_TELEGRAM,
                                CALLED_BY_STARTBUTTON,
                                ALEXA_REQ,
                                TIMER_EXPIRED,
                                PRESS_CONTROL_ON,
                                PRESS_CONTROL_OFF,
                                PUMP_ON,
                                PUMP_STILL_ON,
                                PUMP_OFF,
                                PUMP_FORCED_OFF,
                                LOAD_SUP_ON,
                                LOAD_SUP_OFF };






    #ifdef _I_AM_PIN_INITIALIZATION_CPP_
        const int8_t INPUT_PINS  = 3;
        io_input_pin_struct_t  inpPINs[INPUT_PINS+1];
        io_input_pin_struct_t *pumpState           = &inpPINs[0]; // input
        io_input_pin_struct_t *startButton         = &inpPINs[1]; // input
        io_input_pin_struct_t *pressControlState   = &inpPINs[2]; // input


        const int8_t OUTPUT_PINS = 6;
        io_output_pin_struct_t  outPINs[OUTPUT_PINS+1];
        io_output_pin_struct_t *pressControlRelay   = &outPINs[0];
        io_output_pin_struct_t *pumpHornAlarmRelay  = &outPINs[1];
        io_output_pin_struct_t *activeBuzzer        = &outPINs[2];
        io_output_pin_struct_t *passiveBuzzer       = &outPINs[3];
        io_output_pin_struct_t *pumpLED             = &outPINs[4];
        io_output_pin_struct_t *pressControlLED     = &outPINs[5];

    #else
        // definito in ... pinsInitialization.cpp
        extern io_input_pin_struct_t PINs[];
        extern io_input_pin_struct_t *pumpState; // pin che rileva se la pompa è accesa (c'è un relè in parallelo alla pompa)
        extern io_input_pin_struct_t *startButton; // input
        extern io_input_pin_struct_t *pressControlState; // pin che rileva se il presscontrol è acceso (c'è un relè in parallelo al PC)

        extern io_output_pin_struct_t *pressControlRelay; // pin per attivare relè per accendere il presscontrol
        extern io_output_pin_struct_t *pumpHornAlarmRelay; // pin per attivare relè per accendere il presscontrol
        extern io_output_pin_struct_t *activeBuzzer;
        extern io_output_pin_struct_t *passiveBuzzer;
        extern io_output_pin_struct_t *pumpLED;
        extern io_output_pin_struct_t *pressControlLED;
        // extern io_output_pin_struct_t *LED;
        // extern io_output_pin_struct_t *led_internals;
        // extern io_output_pin_struct_t *relay02;

        extern const char * PROGMEM THRESHOLD_LEVEL_TYPES[];
    #endif


#endif
