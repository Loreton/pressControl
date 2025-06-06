//
// updated by ...: Loreto Notarantonio
// Date .........: 06-06-2025 13.25.46
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <Preferences.h>
#include <fauxmoESP.h>

extern fauxmoESP fauxmo;


// #define PRODUCTION                      1
// #define DEVEL                           2


// #define ESP32_BOARD_TYPE                ESP32_WROOM_32_MODULE
// #define RELEASE_TYPE                    DEVEL
// #warning ESP32_BOARD_TYPE

#define __I_AM_MAIN_CPP__
// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#define LOG_LEVEL_1
#define LOG_LEVEL_2x
#define LOG_LEVEL_3x
#define LOG_LEVEL_4x
#include "@logMacros.h"
#include "@mainStructures.h"
#include "@fileSystem.h"
#include "@pinOperations.h"
#include "@ln_time.h"
#include "@ln_wifi.h"


// ---------------------------------
// - project headers files
// ---------------------------------
#include "@pinDefinitions.h"
#include "@startButton.h"
#include "@pressControl.h"
#include "@pump.h"
#include "@ln_telegram.h"
#include "@ln_Alexa.h"


extern ESP32Time     rtc;
extern struct tm timeinfo; // capire se va bene uno per tutti i moduli oppure mantenerli separati per evitare overwrites


#pragma message(PRINT_MACRO(ln_RELEASE_TYPE))
#pragma message(PRINT_MACRO(ln_PRODUCTION))
#pragma message(PRINT_MACRO(ln_DEVEL))
#pragma message(PRINT_MACRO(ln_ESP32_BOARD_TYPE))
#pragma message(PRINT_MACRO(ln_ESP32_WROOM_32E_MODULE))
#pragma message(PRINT_MACRO(ln_ESP32_WROOM_32E_MODULE_2RELAY))

// #####################################
// --- definizioni timer da tenere sotto controllo

extern io_input_pin_struct_t *startButton;



// #########################################
// # se non c'è ln_time.cpp mi seerver una dummy_Now()
// #########################################
// #define DUMMY_TIME_BUFFER_LENGTH 10
const int8_t DUMMY_TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[DUMMY_TIME_BUFFER_LENGTH];
char *nowTimeDummy() {
    snprintf(temp_buffer_time, DUMMY_TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}


void setup() {
    // Serial.begin(115200);
    lnSERIAL.begin(115200);
     delay(2000);


    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();
    printf1_NFN("turning led on\n");

    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    pinON(pressControlLED);
    time_setup();
    delay(4000);
    printf0_NFN("local time has been set\n");
    pinOFF(pressControlLED);

    // -----------------------------------
    // ------ start WiFi -----------
    // -----------------------------------
    pinON(pressControlLED);
    wifi_Start();
    digitalWrite(pressControlLED->pin, !digitalRead(pressControlLED->pin));
    for (int8_t i=0; i<SSID_ELEMENTS; i++) {
        printf1_NFN("%d - new_ssid: %s\n", i, mySSID[i].ssid);
    }
    delay(1000);
    pinOFF(pressControlLED);

    // -----------------------------------
    // ------ set NTP server
    // -----------------------------------
    pinToggle(pressControlLED);
    NTPInit();
    // digitalWrite(pressControlLED->pin, !digitalRead(pressControlLED->pin));
    delay(1000);
    printf0_NFN("ntp client has been started\n");
    pinOFF(pressControlLED);

    // -----------------------------------
    // ------ set Alexa
    // -----------------------------------
    pinON(pressControlLED);
    AlexaSetup();
    printf0_NFN("Alexa has been started\n");
    delay(1000);
    pinOFF(pressControlLED);


    // -----------------------------------
    // ------ set Telegram
    // -----------------------------------
    setupTelegram();
    sendTelegramGroup((char *)"press /h for help\n");
}
/*
Smart life the user you are inviting are located in different data centers. Devices sharing is not allowed due to data protection policies.
*/



#define LN_RESCAN_WIFI

uint8_t blink_secs=5;

uint32_t wifi_diconnection_elapsed = 0;


io_output_pin_struct_t *ptr;
void loop() {
    static int8_t asyncWifiScan_started=0;
    static bool first_run=true;
    static uint8_t last_pressControlLED_second=0;
    static uint8_t last_pumpLED_second=0;
    timeinfo = rtc.getTimeStruct();

    // -----------------------------------
    // ------ 1st run
    // -----------------------------------
    if (first_run) {
        first_run=false;
        printf0_NFN("starting loop....\n");
        // alignToMinute(); // allineamento al minuto ... non so se mi serve
        // digitalWrite(pressControlLED->pin, pressControlLED->OFF);
        pinOFF(pressControlLED);
        pinOFF(pumpLED);
    }




    // =====================================
    // check if startButton has been pressed.
    // if yes PressControlRelay will be activated
    // =====================================
    uint8_t pressedLevel = startButtonLongPress();
    if (pressedLevel == PRESSED_TIME_01) {
        printf4_NFN("toggling pressControlRelay....\n");
        pressControlRelayToggle(CALLED_BY_STARTBUTTON);
    }
    else if (pressedLevel == PRESSED_TIME_02) {
        printf4_NFN("toggling pumpHornAlarmRelay....\n");
        pinToggle(pumpHornAlarmRelay);
    }


    // ===================================================
    // --- press control Relay status
    // ===================================================
    pressControlRelayStatus();




    // ===================================================
    // --- check for pump
    // ===================================================
    pumpStateForAlarm(); // will read also the pumpState pin


    // ===================================================
    // --- read pin status
    // ===================================================
    readInputPin(pumpState);
    readInputPin(pressControlState);
    readOutputPin(pumpLED);
    readOutputPin(pressControlLED);

    // ---- E R R O R ------
    if (pumpState->is_ON && pressControlState->is_OFF) {
        // pulseGenerator(activeBuzzer, 1000, 500); // buzzer
        pinPeriodPulse(activeBuzzer, 1000, 500); // buzzer
        // pinPulseON(activeBuzzer, 1000, fAUTO_ON_OFF) {
        pinON(pumpHornAlarmRelay);   //HORN acceso fisso
    }
    else {
        // pinOFF(activeBuzzer); // nel caso fossimo andati in allarme
        if (pressControlState->is_ON) {
            if (pressControlLED->is_OFF) {
                printf4_NFN("turning pin ON\n");
                pinON(pressControlLED);
            }
        }
        else {
            if ( (timeinfo.tm_sec%5 == 0) && (timeinfo.tm_sec != last_pressControlLED_second) ) {
                pinPulseON(pressControlLED, 20, fAUTO_ON_OFF);
                last_pressControlLED_second=timeinfo.tm_sec;
            }
        }
    }



    // mantieni pumpLED acceso fisso
    if (pumpState->is_ON && pumpLED->is_OFF) {
        printf4_NFN("turning pin ON\n");
        pinON(pumpLED);
    }
    else if (pumpState->is_OFF) {
        // pumpLED lampeggiante
        if ( (timeinfo.tm_sec%5 == 1) && (timeinfo.tm_sec != last_pumpLED_second) ) {
            printf4_NFN("calling start pulseTime\n");
            pinPulseON(pumpLED, 20, fAUTO_ON_OFF);
            last_pumpLED_second=timeinfo.tm_sec;
        }
    }




    if (isSecondOClock()) {
        handleTelegram();


        // ---------------------------------
        // - check for Alexa connection
        // fauxmoESP uses an async TCP server but a sync UDP server
        // Therefore, we have to manually poll for UDP packets
        // ---------------------------------
        chackAlexa();
        ptr = pressControlRelay;
        if (ptr->alexa_request) {
            pressControlRelaySet(ptr->alexa_status, ALEXA_REQ);
            ptr->alexa_request = false;
        }



        // ---------------------------------
        // - check for WiFi connection
        // ---------------------------------
        if (wifi_isConnected()) {
            wifi_diconnection_elapsed = millis();
        }
        else {
            printf0_NFN("connection lost (elapsed: %ld)\n", (millis() - wifi_diconnection_elapsed) );

            if (millis() - wifi_diconnection_elapsed > 5*60*1000) { // 5 minuti
                delay(10000);
                ESP.restart();
            }
            else if (millis() - wifi_diconnection_elapsed > 2*60*1000) { // 2 minuti
                wifi_reconnect();
            }
        }

    }






    // ---------------------------------
    // actions on 01 minutes timer
    // ---------------------------------
    if (isMinuteOClock()) {
        printf1_NFN("minute o'clock routine\n");

        if (wifi_isConnected()) {
            printf0_NFN("WiFi status:\n");
            printConnection();
            // pingHost("www.google.com");
            // pingHost("api.telegram.com");
        }

        #if 0
        if (! wifi_isConnected()) {
            printf0_NFN("connection lost\n");
            if (millis() - wifi_diconnection_elapsed > 2*60*1000) {
                wifi_reconnect();
            }
        }
        else {
            wifi_diconnection_elapsed = millis();
            printf0_NFN("WiFi status:\n");
            printConnection();
            pingHost("www.google.com");
            if (! pingHost("api.telegram.com")) {
                // setupTelegram();
                // sendTelegramGroup((char *)"press /h for help\n");
            }
        }
        #endif







        #ifdef LN_RESCAN_WIFI
        // ---------------------------------
        // if wifiScan has been started check for results
        // ---------------------------------
        if (asyncWifiScan_started > 0) {
            asyncWifiScan_started++; // aumentiamo
            int16_t networksFound = wifi_scanComplete();
            if (networksFound == -1) {
                printf1_NFN("discovery networks still running!!\n");
            }
            else {
                printf1_NFN("networks found: %d\n", networksFound);
                connectOnScanResult(networksFound);
                asyncWifiScan_started=0; // chiudiamo il flag
                wifi_scanDelete();
            }

            if (asyncWifiScan_started > 4) { // inutile dopo 4 minuti....
                asyncWifiScan_started=0; // chiudiamo il flag
                wifi_scanDelete();
            }
        }
        #endif
    }


    #ifdef LN_RESCAN_WIFI
    // ---------------------------------
    // actions every 15 minutes (at any quarter)
    // ---------------------------------
    if (isQuarterOClock()) {
        printf1_NFN("it's a quarter of hour routine\n");
        wifi_asyncScan();
        asyncWifiScan_started=1;
    }
    #endif



}


