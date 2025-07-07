//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.44.04
//


#pragma once
    #include "pin_Definitions.h"
    #include "PinController_Struct.h"
    // #include "PinController_Struct.h"
    #include "ButtonLongPress_Struct.h"







    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        ButtonLongPress_Struct startButton;
        ButtonLongPress_Struct pumpState;
        PinController_Struct   activeBuzzer;
        PinController_Struct   pressControlLED;
        PinController_Struct   pumpLED;


    #else
        // extern ButtonDebounced_Class startButton;
        extern ButtonLongPress_Struct startButton;
        extern ButtonLongPress_Struct pumpState;
        extern PinController_Struct   activeBuzzer;
        extern PinController_Struct   pressControlLED;
        extern PinController_Struct   pumpLED;

    #endif




// -------------------------------------------
// - Functions prototypse
// -------------------------------------------
    // --- pinsInitialization.cpp
    void pinsInitialization(void);


    // --- actionProcess.cpp
    // void startButton_action(void);


