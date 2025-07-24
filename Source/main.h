//
// updated by ...: Loreto Notarantonio
// Date .........: 24-07-2025 17.54.17
//


#pragma once
    #include "pin_Definitions.h"
    #include "LedController_Struct.h"
    #include "ButtonLongPress_Struct.h"
    #include "PassiveBuzzer_Struct.h"
    #include "RelayManager_Struct.h"

    #include "callBackPrototypes.h" // per functions protoype
    #include "processPrototypes.h" // per functions protoype







    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        ButtonLongPress_Struct startButton; // pulsante per accendre il pressControl tramite Relay del ESP32
        ButtonLongPress_Struct pumpState;   // pin per controllare lo stato della pompa
        ButtonLongPress_Struct pressControlState; // pin per controllare lo stato del pressContro

        LedController_Struct   activeBuzzer;
        LedController_Struct   pressControlLED;
        LedController_Struct   pumpLED;

        PassiveBuzzer_Struct   passiveBuzzer;

        RelayManager_Struct    pressControlRelay;
        RelayManager_Struct    magnetoTermicoRelay;


    #else
        // extern ButtonDebounced_Class startButton;
        extern ButtonLongPress_Struct startButton;
        extern ButtonLongPress_Struct pumpState;
        extern ButtonLongPress_Struct pressControlState;

        extern LedController_Struct   activeBuzzer;
        extern LedController_Struct   pressControlLED;
        extern LedController_Struct   pumpLED;

        extern PassiveBuzzer_Struct   passiveBuzzer;

        extern RelayManager_Struct    pressControlRelay;
        extern RelayManager_Struct    magnetoTermicoRelay;

    #endif




// -------------------------------------------
// - Functions prototypse
// -------------------------------------------
    // --- pinsInitialization.cpp
    void pinsInitialization(void);
    void startAlarmActions(void) ;


    // --- actionProcess.cpp
    // void startButton_action(void);


