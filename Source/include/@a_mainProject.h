//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 18.42.12
//

#include <Arduino.h>

#ifndef __PROJECT_STRUCTURES_H__
    #define __PROJECT_STRUCTURES_H__
    #include "@a_decisionalVariables.h"
    #include "@pin_Structures.h"



    // enum ErrorCode   : uint8_t { ERROR_01 = 1, ERROR_10 = 10 };


    // enum displayData : uint8_t { NO_DISPLAY=0, DISPLAY_STATUS}; // come parametro per fare il display in alcune funzioni (es.: is_acted())

    // enum caller      : uint8_t { NO_TELEGRAM = 0,
    //                             CALLED_BY_TELEGRAM,
    //                             CALLED_BY_STARTBUTTON,
    //                             ALEXA_REQ,
    //                             TIMER_EXPIRED,
    //                             PRESS_CONTROL_ON,
    //                             PRESS_CONTROL_OFF,
    //                             PUMP_ON,
    //                             PUMP_STILL_ON,
    //                             PUMP_OFF,
    //                             PUMP_FORCED_OFF,
    //                             LOAD_SUP_ON,
    //                             LOAD_SUP_OFF };



    // Struttura per mantenere lo stato di ogni pulsante.
    typedef struct {
        int pin;                  // Il pin GPIO a cui è collegato il pulsante.
        const char* name;         // Nome del pulsante per identificazione (opzionale, utile per debugging).
        int pressedLogicLevel;    // Livello logico che indica il pulsante premuto (LOW o HIGH).
        bool lastButtonState;     // Ultima lettura RAW del pin.
        unsigned long lastDebounceTime; // Ultimo momento in cui il pin ha cambiato stato RAW.
        bool buttonPressed;       // Stato debounced: true se premuto, false se rilasciato.
    } deBouncedButton_t;





    #ifdef _I_AM_PIN_INITIALIZATION_CPP_
        #define INPUT_PINS  3
        // deBouncedButton_s startButton;
        // deBouncedButton_s startButton2;


        deBouncedButton_t  inpPINs3[INPUT_PINS+1];
        deBouncedButton_t *startButton3  = &inpPINs3[0]; // Dichiarazione della struttura per il nostro pulsante.
        deBouncedButton_t *startButton4  = &inpPINs3[1]; // Dichiarazione della struttura per il nostro pulsante.
        // deBouncedButton_t *pumpState           = &inpPINs[0]; // input
        // deBouncedButton_t *pressControlState   = &inpPINs[2]; // input

    #else
        // extern deBouncedButton_s startButton;
        // extern deBouncedButton_s startButton2;
        extern deBouncedButton_t *startButton3; // Dichiarazione della struttura per il nostro pulsante.
        extern deBouncedButton_t *startButton4; // Dichiarazione della struttura per il nostro pulsante.
        // extern deBouncedButton_t *startButton;  // input
    #endif


    #ifdef _XXXXXXXX____
        // altro modo di lavorare con le strutture ma mi trovo meglio con il metodo del typedef


        // // Struttura per mantenere lo stato di ogni pulsante.
        struct deBouncedButton_s {
            int pin;                  // Il pin GPIO a cui è collegato il pulsante.
            const char* name;         // Nome del pulsante per identificazione (opzionale, utile per debugging).
            int pressedLogicLevel;    // Livello logico che indica il pulsante premuto (LOW o HIGH).
            bool lastButtonState;     // Ultima lettura RAW del pin.
            unsigned long lastDebounceTime; // Ultimo momento in cui il pin ha cambiato stato RAW.
            bool buttonPressed;       // Stato debounced: true se premuto, false se rilasciato.
        };

        #ifdef _I_AM_PIN_INITIALIZATION_CPP_
            deBouncedButton_s startButton;
            deBouncedButton_s startButton2;
        #else
            extern deBouncedButton_s startButton;
            extern deBouncedButton_s startButton2;
        #endif
        // si usa l'& nel parametro della funzione ed il punto per gli attributi
        void setup_deBouncedButton2(deBouncedButton_s &btn, int pin, const char* name, int pressedLogicLevel);

    #endif


#endif
