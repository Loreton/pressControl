//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 17.08.25
//


#pragma once
    #include "pin_Definitions.h"
    // #include "outPinController_Class.h"
    #include "outPinController_Class.h"
    #include "ButtonLongPress_Struct.h"
    #include "passiveBuzzer_Class.h"
    #include "relayManager_Class.h"

    #include "callBackPrototypes.h" // per functions protoype
    #include "processPrototypes.h" // per functions protoype
    #include "lnLogger_Class.h" // per functions protoype







    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        ButtonLongPress_Struct startButton; // pulsante per accendre il pressControl tramite Relay del ESP32
        ButtonLongPress_Struct pumpState;   // pin per controllare lo stato della pompa
        ButtonLongPress_Struct pressControlState; // pin per controllare lo stato del pressContro

        outPinController_Class   activeBuzzer;
        outPinController_Class   pressControlLED;
        outPinController_Class   pumpLED;

        PassiveBuzzer_Class   passiveBuzzer;

        RelayManager_Class    pressControlRelay;
        RelayManager_Class    magnetoTermicoRelay;


    #else
        // extern ButtonDebounced_Class startButton;
        extern ButtonLongPress_Struct startButton;
        extern ButtonLongPress_Struct pumpState;
        extern ButtonLongPress_Struct pressControlState;

        extern outPinController_Class   activeBuzzer;
        extern outPinController_Class   pressControlLED;
        extern outPinController_Class   pumpLED;

        extern PassiveBuzzer_Class   passiveBuzzer;

        extern RelayManager_Class    pressControlRelay;
        extern RelayManager_Class    magnetoTermicoRelay;

    #endif




// -------------------------------------------
// - Functions prototypse
// -------------------------------------------
    // --- pinsInitialization.cpp
    void pinsInitialization(void);
    void startAlarmActions(void) ;


    // --- actionProcess.cpp
    // void startButton_action(void);


