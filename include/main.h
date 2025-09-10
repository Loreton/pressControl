//
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 15.35.21
//


#pragma once

    // #include "a_decisionalVariables.h"
    #include    "functionPrototypes.h" // per functions protoype
    #include    "pin_Definitions.h"


    // #include    <lnLogger_Class.h>
    #include    <ButtonLongPress_Class.h>
    #include    <outPinController_Class.h>
    #include    <passiveBuzzer_Class.h>
    #include    <relayManager_Class.h>
    #include    <telegramBot_Class.h>
    #include    <WiFiManager_Class.h>
    #include    <LnTime_Class.h>





    // ---  TEST
    #if ln_RELEASE_TYPE == ln_DEVEL
        #define PUMP_PHASE_01                      5*1000UL  // milliSeconds
        #define PUMP_PHASE_02                     15*1000UL // milliSeconds
        #define PUMP_PHASE_03                     20*1000UL // milliSeconds
        #define PUMP_PHASE_04                     25*1000UL // milliSeconds
        #define BEEP_MULTIPLICATION_FACTOR         1*1000UL  // milliSeconds

        #define ACTION_STATUS_DISPLAY_INTERVAL 2*60*1000UL  // xx minuti
        #define PRESS_CONTROL_PIN_MAX_TIME     1*60*1000UL // xx minuti
        #define PRESS_CONTROL_RELAY_MAX_TIME   2*60*1000UL // xx minuti
        #define MAGNETOTERMIC_RELAY_PULSETIME     5*1000UL  // tempo magnetotermico relay sarà off


    #elif ln_RELEASE_TYPE == ln_PRODUCTION
        #define PUMP_PHASE_01                   1*60*1000UL   // milliSeconds
        #define PUMP_PHASE_02                   2*60*1000UL  // milliSeconds
        #define PUMP_PHASE_03                   3*60*1000UL  // milliSeconds
        #define PUMP_PHASE_04                   4*60*1000UL  // milliSeconds
        #define BEEP_MULTIPLICATION_FACTOR        2*1000UL  // milliSeconds

        #define ACTION_STATUS_DISPLAY_INTERVAL  5*60*1000UL  // xx minuti
        #define PRESS_CONTROL_PIN_MAX_TIME     30*60*1000UL  // tempo in cui il pressControl starà acceso.
        #define PRESS_CONTROL_RELAY_MAX_TIME   30*60*1000UL  // tempo in cui il relay interno starà acceso.
        #define MAGNETOTERMIC_RELAY_PULSETIME      5*1000UL  // tempo magnetotermico relay sarà off
    #endif



    #ifdef __I_AM_MAIN_CPP__
        #include "wifiManager_ssid_credentials.h"
        #include "@ln_esp32_telegram_credentials.h"



        // L'inizializzazione avviene tramite il costruttore.
        ButtonLongPress_Class            startButton; // pulsante per accendre il pressControl tramite Relay del ESP32
        ButtonLongPress_Class            pumpState;   // pin per controllare lo stato della pompa
        ButtonLongPress_Class            pressControl; // pin per controllare lo stato del pressContro

        outPinController_Class           activeBuzzer;
        outPinController_Class           pressControlLED;
        outPinController_Class           pumpLED;

        PassiveBuzzer_Class              passiveBuzzer;

        RelayManager_Class               pressControlRelay;
        RelayManager_Class               magnetoTermicoRelay;

        bool fPressControlTimeExausted = false; // indica che il pressContro ha terminato il tempo massiomo di ON
        bool fPUMP_ALARM               = false;
        bool fAscendent                = true;
        bool fDiscendent               = false;

        bool f3MinutesModulo          = false;
        bool f10SecondsModulo          = false;
        bool f30SecondsModulo          = false;

        bool f2MinutesModulo           = false;
        bool f30MinutesModulo          = false;

        bool fonDay          = false;
        bool fonHour          = false;
        bool fonMinute          = false;
        bool fonSecond          = false;

        // Frequenze per una scala Do Maggiore (approssimate)
        int C_major_scale[] = {
            262, // C4
            294, // D4
            330, // E4
            349, // F4
            392, // G4
            440, // A4
            494, // B4
            523  // C5
        };
        // int num_notes_C_major = sizeof(C_major_scale) / sizeof(C_major_scale[0]);
        int C_major_num_notes = sizeof(C_major_scale) / sizeof(C_major_scale[0]);

        // Crea un'istanza della classe telegramBot_Class
        TelegramBot_Class myBot;

        // Crea un'istanza della classe WiFiManager
        WiFiManager_Class myWiFiManager;
        // LnTime_Class lnTime; definito in  LnTime_Class.cpp






    #else
        extern ButtonLongPress_Class  startButton;             // pulsante per accendre il pressControl tramite Relay del ESP32
        extern ButtonLongPress_Class  pumpState;               // pin per controllare lo stato della pompa
        extern ButtonLongPress_Class  pressControl;            // pin per controllare lo stato del pressContro

        extern outPinController_Class activeBuzzer;
        extern outPinController_Class pressControlLED;
        extern outPinController_Class pumpLED;

        extern PassiveBuzzer_Class    passiveBuzzer;

        extern RelayManager_Class     pressControlRelay;
        extern RelayManager_Class     magnetoTermicoRelay;

        extern bool                   fPressControlTimeExausted; // indica che il pressContro ha terminato il tempo massiomo di ON
        extern int C_major_scale[];
        extern int  C_major_num_notes;
        extern bool fPUMP_ALARM;
        extern bool fAscendent;
        extern bool fDiscendent;

        extern bool f3MinutesModulo;
        extern bool f10SecondsModulo;
        extern bool f30SecondsModulo;

        extern bool f2MinutesModulo;
        extern bool f30MinutesModulo;
        extern bool fonDay;
        extern bool fonHour;
        extern bool fonMinute;
        extern bool fonSecond;



        extern TelegramBot_Class      myBot;
        extern WiFiManager_Class      myWiFiManager;
        extern LnTime_Class           lnTime;

    #endif




// -------------------------------------------
// - Functions prototypse
// -------------------------------------------
    // --- pinsInitialization.cpp
    void pinsInitialization(void);
    void startAlarmActions(void) ;
    void resetAlarmActions(bool synchBlinking=false) ;


    // --- actionProcess.cpp
    // void startButton_action(void);


