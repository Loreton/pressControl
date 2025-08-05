//
// updated by ...: Loreto Notarantonio
// Date .........: 05-08-2025 11.43.55
//


#pragma once

    #include "lnLogger_Class.h"
    #include "functionPrototypes.h" // per functions protoype

    #include "ButtonLongPress_Class.h"
    #include "outPinController_Class.h"
    #include "passiveBuzzer_Class.h"
    #include "pin_Definitions.h"
    #include "relayManager_Class.h"


    #define ACTION_STATUS_DISPLAY_INTERVAL 30*1000
    #define PRESS_CONTROL_PIN_MAX_TIME     3*60*1000 // milliSeconds
    #define PRESS_CONTROL_RELAY_MAX_TIME   3*60*1000 // milliSeconds
    #define PUMP_PHASE_01   5*1000                  // milliSeconds
    #define PUMP_PHASE_02   15*1000                 // milliSeconds
    #define PUMP_PHASE_03   20*1000                 // milliSeconds
    #define PUMP_PHASE_04   25*1000                 // milliSeconds




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
        bool                fPressControlTimeExausted = false; // indica che il pressContro ha terminato il tempo massiomo di ON


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

        extern bool                fPressControlTimeExausted; // indica che il pressContro ha terminato il tempo massiomo di ON
    #endif




// -------------------------------------------
// - Functions prototypse
// -------------------------------------------
    // --- pinsInitialization.cpp
    void pinsInitialization(void);
    void startAlarmActions(void) ;


    // --- actionProcess.cpp
    // void startButton_action(void);


