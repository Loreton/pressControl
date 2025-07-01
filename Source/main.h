//
// updated by ...: Loreto Notarantonio
// Date .........: 01-07-2025 18.17.37
//


#pragma once
    // doesn't work
    // #include "@debouncedButton_sClass.h"
    // #include "@logMacros.h"


    #include "a_decisionalVariables.h"
    #include "pin_Definitions.h"


    #include "@ledController_Class.h"
    // #include "@buttonDebounced_Class.h"
    #include "@buttonLongPress_Class.h"



    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        ButtonLongPress_Class startButton;
        ButtonLongPress_Class pumpState;
        LedController_Class   activeBuzzer;
        LedController_Class   pressControlLED;
        LedController_Class   pumpLED;



    #else
        // extern ButtonDebounced_Class startButton;
        extern ButtonLongPress_Class startButton;
        extern ButtonLongPress_Class pumpState;
        extern LedController_Class   activeBuzzer;
        extern LedController_Class   pressControlLED;
        extern LedController_Class   pumpLED;

    #endif




// -------------------------------------------
// - Functions prototypse
// -------------------------------------------
    // --- pinsInitialization.cpp
    void pinsInitialization(void);


    // --- actionProcess.cpp
    void startButton_action(void);


