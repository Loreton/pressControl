//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 08.28.38
//


#pragma once
    #include "pin_Definitions.h"
    #include "outPinController_Class.h"
    #include "passiveBuzzer_Class.h"
    #include "relayManager_Class.h"

    #include "callBackPrototypes.h" // per functions protoype
    #include "processPrototypes.h" // per functions protoype
    #include "lnLogger_Class.h" // per functions protoype
    #include "ButtonLongPress_Class.h"







    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        ButtonLongPress_Class startButton; // pulsante per accendre il pressControl tramite Relay del ESP32
        ButtonLongPress_Class pumpState;   // pin per controllare lo stato della pompa
        ButtonLongPress_Class pressControlState; // pin per controllare lo stato del pressContro

        outPinController_Class   activeBuzzer;
        outPinController_Class   pressControlLED;
        outPinController_Class   pumpLED;

        PassiveBuzzer_Class   passiveBuzzer;

        RelayManager_Class    pressControlRelay;
        RelayManager_Class    magnetoTermicoRelay;


    #else
        extern ButtonLongPress_Class startButton; // pulsante per accendre il pressControl tramite Relay del ESP32
        extern ButtonLongPress_Class pumpState;   // pin per controllare lo stato della pompa
        extern ButtonLongPress_Class pressControlState; // pin per controllare lo stato del pressContro

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


