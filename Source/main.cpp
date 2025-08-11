//
// updated by ...: Loreto Notarantonio
// Date .........: 11-08-2025 10.59.36
//




#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    "lnGlobalVars.h"
#include    "lnSerialRead.h" // waitFor...

// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"
#include "lnTime_Class.h"
#include "WiFiManager_Class.h"


#define __LOAD_SSIDS_CPP__
    #include "wifiManager_ssid_credentials.h"
#undef __LOAD_SSIDS_CPP__

// extern "C" void lwip_hook_ip6_input() {
    // Funzione hook vuota per risolvere il problema di linking
    // undefined reference to lwip_hook_ip6_input'
// }


size_t initialMemory;
size_t finalMemory;


// Crea un'istanza della classe WiFiManager
WiFiManager_Class myWiFiManager;


#define VERSION_LENGTH 40
char pressControlVersion[VERSION_LENGTH+1];
void setup() {
    initialMemory = ESP.getFreeHeap();

    snprintf(pressControlVersion, VERSION_LENGTH, "Version_2025-07 - rel_type: %d", ln_RELEASE_TYPE);

    Serial.begin(115200);
    delay(1000);
    lnLog.init();

    LOG_INFO("%s", pressControlVersion);

    // -----------------------------------
    // ------ WiFi
    // -----------------------------------
    myWiFiManager.init(myNetworks, sizeof(myNetworks) / sizeof(myNetworks[0]));

    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    lnTime.setup(); // Chiama il metodo setup della tua istanza di LnTime

    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();



    // String message = "Ciao dal tuo ESP32! Sono le " + String(millis() / 1000) + " secondi.";
    // sendMessageToTelegram(message);



    // ---------------- calcolo memoria
    finalMemory = ESP.getFreeHeap();
    LOG_TRACE("memoria (bytes): initial=%ld - final=%ld - occupied=%ld", initialMemory, finalMemory, (initialMemory - finalMemory)); // Stima RAM allocata


}



// Definisce i possibili tipi di condizioni
enum ActionState : uint8_t {
    pcOFF_pumpOFF = 0,      // tutto spento.
    pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    pcON_pumpON,     // rele esterno - Pressione lunga.
    // a05_relayON_pcOFF_pumpOFF,      // solo relay interno  - ERROR.
    // a06_relayON_pcOFF_pumpON,    // solo la pompa è acessa. Anomalo. Non dovrebbe mai accadere
    // a07_relayON_pcON_pumpOFF,    // rele esterno - PressControl ON (con il rele esterno)
    // a08_relayON_pnON_pumpON,     // rele esterno - Pressione lunga.
    pumpAlarm,
    // pcAlarm,
} ;


// const PROGMEM char *alarmState[] = {"[EXT_RELAY] ALL_OFF", "[EXT_RELAY] ABNORMAL_pumpON", "[EXT_RELAY] pcON", "[EXT_RELAY] PC+PUMP ON", "[INT_RELAY] ALL_OFF", "[INT_RELAY] ABNORMAL_pumpON", "[INT_RELAY] pcON", "[INT_RELAY] PC+PUMP ON", };

bool    first_run=true;
// bool    fAlarm=false;
uint8_t actionState=0;
uint8_t lastActionState=1;
uint32_t lastDisplayTime=0;
uint32_t actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;

void loop() {
    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);

    char durationBUFFER[16];
    uint32_t duration;
    uint32_t now=millis();
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }



    // -----------------------------------
    // ------ refresh dei vari oggetti
    // -----------------------------------
    activeBuzzer.update();
    passiveBuzzer.update();
    pressControlLED.update();
    pumpLED.update();
    pressControlRelay.update();
    magnetoTermicoRelay.update();

    myWiFiManager.update();
    lnTime.update();
    if (lnTime.everyXminutes(30)) {
        char message[100];
        // snprintf(message, 99, "Ciao dal tuo ESP32! Sono le %lu secondi.", millis() / 1000);
        uint8_t msg_len = snprintf(message, 99, lnTime.timeStamp(message, 99));
        msg_len += snprintf(message+msg_len, 99, "\nCiao dal tuo ESP32!");
        sendMessageToTelegram(message);
    }


    /**
     * Leggi lo stato dello startButton
     * Se è stato rilasciato
    */
    startButton.pressingLevelNotification(startButtonNotificationCB);
    if (startButton.released()) {
        startButtonHandler(startButton.currentPressLevel());
        startButton.reset();
    }




    /**
     * Leggi lo stato della pompa
     * Se è stato rilasciato
    */
    pumpState.pressingLevelNotification(pumpNotificationCB);
    if (pumpState.released()) {
        pumpHandler(&pumpState);
        pumpState.reset();
    }


    /**
     * Leggi lo stato del pressControl
     * Se è stato rilasciato
    */
    pressControl.pressingLevelNotification(pressControlNotificationCB);
    if (pressControl.released()) {
        pressControl.reset();
    }


    // -----------------------------------
    // ------ Action
    // -----------------------------------
    uint8_t relayStatus = pressControlRelay.isActive();
    uint8_t pumpStatus  = pumpState.isPressed();
    uint8_t pcStatus    = pressControl.isPressed();

    actionState = (pcStatus*2) + (pumpStatus*1);
    if (fPUMP_ALARM) {
        actionState = pumpAlarm;
    }


    if ( (actionState != lastActionState) || (now - lastDisplayTime) > actionStateDisplayInterval) { // facciamo comunque il display ogni 15 secondi
        lastActionState=actionState;
        lastDisplayTime=now;

        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, "RELAY", "PRESS-CONTROL", "PUMP");
        LOG_INFO("actionState [%02d]: %7s %16s %5s", actionState, str_OnOff[relayStatus], str_OnOff[pcStatus], str_OnOff[pumpStatus]);

    }

    switch (actionState) {

        // status normale in attesa che si accenda il PC
        case pumpAlarm:
            LOG_ERROR("Pump Alarm");
            startAlarmActions();
            break;


        // status normale in attesa che si accenda il PC
        case pcOFF_pumpOFF:
            if (relayStatus) {
                // non può essere il rele on ed il PC off
                LOG_ERROR("Relè OFF quando invece il PC è ON");
                startAlarmActions();
            } else {
                // LOG_INFO("Situazione normale");
                resetAlarmActions();
            }
            break;


        // non può essere la pompa ON ed il PC off
        case pcOFF_pumpON:
            LOG_ERROR("Pump ON quando il PC è OFF.");
            startAlarmActions();
            break;

        // status normale in attesa che si accenda la pompa
        case pcON_pumpOFF:
            actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;
            pressControlLED.on();         // accendiamo fisso il LED
            pumpLED.off(); // facciamoòp lampeggiare
            break;


        // status normale con la pompa accesa
        case pcON_pumpON:
            actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;
            pressControlLED.on();
            pumpLED.on();
            break;

        default:
            break;

    } // endo of switch


}



void startAlarmActions() {
    static uint32_t relay_delay=MAGNETOTERMIC_RELAY_PULSETIME + 3000;
    static uint32_t last_relay_time;
    if (! fPUMP_ALARM) {
        LOG_ERROR("Starting Recovery Actions for Alarm."); // NO perchè compare ad ogni giro di loop
        last_relay_time=0;
    }

    fPUMP_ALARM=true;
    pressControlRelay.off();
    // tentiamo di togliere corrente al magnetotermico
    // tra un tentativo e l'altro aspettiamo un pò
    if ( (millis() - last_relay_time) > relay_delay ) {
        magnetoTermicoRelay.startPulse(MAGNETOTERMIC_RELAY_PULSETIME); // non abbiamo armi. proviamo a chiidere il relay esterno togliendo alimentazione
        last_relay_time = millis();
    }
    pumpLED.blinking(300, 300);
    pressControlLED.blinking(300, 300);
    activeBuzzer.blinking(300, 300);
    actionStateDisplayInterval=2000; // ogni due secondi
}



void resetAlarmActions() {
    if (fPUMP_ALARM) {
        LOG_INFO("Recovery Actions for Alarm Ended."); // NO perchè compare ad ogni giro di loop
        fPUMP_ALARM=false;
    }
    actionStateDisplayInterval=ACTION_STATUS_DISPLAY_INTERVAL;
    pressControlLED.blinking(1000, 3000);
    pumpLED.blinking(1000, 3000);
    activeBuzzer.reset();
    passiveBuzzer.myNoTone();
    pressControlRelay.off();       // spegniamo epr sicurezza il relay interno
    magnetoTermicoRelay.off();
}
