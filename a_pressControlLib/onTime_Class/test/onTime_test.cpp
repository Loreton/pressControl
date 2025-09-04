//
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 09.48.22
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include <lnLogger_Class.h>

#include "OnTime_Class.h"
ESP32Time rtc;
struct tm timeinfo;

void atSecond(void) {
    // LOG_WARN("atSecond:  %d", value);
    static const int8_t at_array[] = {0, 1, 5, 15, 30, 45};
    for (int i = 0; i < sizeof(at_array); i++) {
        int8_t value = at_array[i];
        if (onTime.atSecond(value)) {
            LOG_WARN("atSecond:  %d", value);
        }
    }
}


void onSecondModulo(void) {
    static const int8_t array[] = {0, 12, 15, 30, 45, 59};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];

        if (onTime.onSecondModulo(value)) {
            LOG_WARN("onSecondModulo: %d", value);
        }
    }
}


const std::vector<uint8_t> seconds = {0, 1, 5, 15, 30, 45};
const std::vector<uint8_t>& mySecs = seconds;
void atSecondVector(void) {
    static bool first_run = true;
    if (first_run) {
        onTime.setAtSecondFlag(seconds);
        first_run = false;
    }

    for (uint8_t sec : mySecs) {
        if (onTime.getAtSecondFlag(sec)) {
            LOG_NOTIFY("flagSecond: %d", sec);
        }
    }
}



void setup() {
    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    Serial.println("Sono onTime TEST");
}



bool    first_run=true;



// #############################################################
// #
// #############################################################
int8_t counter=0;
void loop() {
    timeinfo = rtc.getTimeStruct(); // lo facciamo qui una volta sola....
    onTime.update();


    if (onTime.onSecond()) {
        // LOG_INFO("It's a new second!");
    }

    if (onTime.onMinute()) {
        Serial.println();
    }

    atSecond();
    onSecondModulo();
    atSecondVector();


    if (first_run) {
        first_run=false;
        LOG_INFO("fine primo loop....\n\n");
    }
    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


