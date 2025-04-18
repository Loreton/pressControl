//
// updated by ...: Loreto Notarantonio
// Date .........: 20-02-2025 10.19.42
//

// Arduino board: ESP32-WROOM-DA Module
#include <Arduino.h> // in testa anche per le definizioni dei type



#include "@ln_time.h" // per la funzione nowTime()

#ifndef __MAIN_H__
    #define __MAIN_H__
    #define SERIAL_BAUDRATE 115200

    // #define Now nowTime




    // ==============================================
    // = constants
    // ==============================================
    #define MAX_PHASES              3
    #define PHASE_INTERVAL          15*1000L    // number of mseconds between Buzzer
    #define PHASE_ALARM_INTERVAL    PHASE_INTERVAL/5
    #define PHASE_MIN_INTERVAL      1*1000    // minimo intervallo di Buzzer
    #define PHASE_STEP_DOWN         PHASE_INTERVAL/30    // step con cui deve scendere l'intervallo per ogni fase



    // extern const char *BLANK_2;
    // extern const char *BLANK_4;
    // extern const char *BLANK_6;
    // extern const char *BLANK_8;

    extern const char *str_ON;
    extern const char *str_OFF;
    extern const char *str_INPUT;
    extern const char *str_INPUT_PULLUP;
    extern const char *str_OUTPUT;
    extern char  PROGMEM temp_buffer_time[];







#endif



