/*
// updated by ...: Loreto Notarantonio
// Date .........: 19-04-2025 14.31.25
*/
/*
 * Rui Santos
 * Complete Project Details http://randomnerdtutorials.com
*/


#ifndef __MAIN_PROJECT_H__
    #define __MAIN_PROJECT_H__

    #define SERIAL_BAUDRATE 115200

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



    // definito in ...
    // extern const char *str_action[];
    // extern const char *str_pinLevel[];
    // extern const char *str_TrueFalse[];
    // extern const char *str_OffOn[];
    // extern const char *str_INPUT;
    // extern const char *str_INPUT_PULLUP;
    // extern const char *str_OUTPUT;
    // extern const int8_t INPUT_PINS;
    // extern const int8_t OUTPUT_PINS;



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

