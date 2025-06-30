//
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 16.26.02
//


#pragma once
    // doesn't work
    // #include "@debouncedButton_sClass.h"
    // #include "@logMacros.h"


    #include "a_decisionalVariables.h"
    #include "pin_Definitions.h"


    #include "@ledController_Class.h"
    #include "@buttonDebounced_Class.h"



    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        ButtonDebounced_Class startButton;
        LedController_Class   activeBuzzer;
        LedController_Class   pressControlLED;
        LedController_Class   pumpLED;

        // ButtonLongPress_Class startButton;
        // ButtonLongPress_Class pressControlState;
        // ButtonLongPress_Class pumpState;
        // RelayManager_Class    pressControlRelay;
        // PassiveBuzzer_Class   passiveBuzzer;

    #else
        extern ButtonDebounced_Class startButton;
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


