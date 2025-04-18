//
// updated by ...: Loreto Notarantonio
// Date .........: 25-02-2025 15.44.59
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <Preferences.h>
// #include <WiFi.h>


#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <Preferences.h>
// #include <WiFi.h>


// --- direttamente correlati
#include "@lnMacros.h"
#include "@ln_wifi.h"
#include "@pinDefinitions.h"
#include "@structures.h"



// --- loreto others
// #include "@wifiCredentials.h"
// #include "@ln_MillisTimer.h"
// #include "@ln_time.h"
// #include "@ln_telegram.h"
#include "@pinDebug.h"
// #include "@startButton.h"
// #include "@pump.h"
// #include "@pressControl.h"



// extern ESP32Time     rtc;
// extern struct tm timeinfo; // capire se va bene uno per tutti i moduli oppure mantenerli separati per evitare overwrites

// ---------------------------------
// macros Aliases
// ---------------------------------
// #define print           ln_print
#define printf0          lnPrintF
#define printf0_FN        lnPrintF_FN
#define printf0_NFN        lnPrintF_NowFN

#define print_ln         lnPrintLN
#define print_ln2        /*lnPrintLN*/
#define printf2_FN        lnPrintF_FN
#define printf2_NFN        lnPrintF_NowFN


// structMillisTimer_t pumpStateTimer;  structMillisTimer_t *p = &pumpStateTimer;

// #########################################
// # se non c'è ln_time.cpp mi seerver una dummy_Now()
// #########################################
const int8_t TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[TIME_BUFFER_LENGTH];
char *nowTimeDummy() {
    snprintf(temp_buffer_time, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}



// structMillisTimer_t timer1MIN;  structMillisTimer_t *timer1min = &timer1MIN;
// structMillisTimer_t timer15MIN; structMillisTimer_t *timer15min = &timer15MIN;


void setup() {
    Serial.begin(115200);
    delay(2000);
    print_ln();

    // -----------------------------------
    // --- "pins_Initialization.cpp"
    // -----------------------------------
    pinsInitialization();
    printf0_NFN("turnig led on\n");
    digitalWrite(LED->pin, LED->ON);



}



// ##########################################################
// #
// ##########################################################
uint32_t loop_millis;
uint32_t last_loop_time;

int8_t asyncWifiScan_started=0;
bool first_run=true;





void loop() {

    // =====================================
    // = process PINs
    // =====================================
    checkPin_forDebug(startButton, first_run);
    checkPin_forDebug(pressControlState, first_run);
    checkPin_forDebug(pumpState, first_run);
    // printf0("\n\n");
    delay(100);


    // -----------------------------------
    // ------ 1st run
    // -----------------------------------
    if (first_run) {
        first_run=false;
        printf2_NFN("first run ended....\n");
        digitalWrite(LED->pin, LED->OFF);
    }




}


