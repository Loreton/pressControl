//
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 17.00.43
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include <lnLogger_Class.h>



#define ON_TIME_CLASS_SECONDS_VECTORx
#define ON_TIME_CLASS_MINUTES_VECTORx
#define ON_TIME_CLASS_HOURS_VECTORx
#include "OnTime_Class.h"


ESP32Time rtc;
struct tm timeinfo;


// ================ Seconds ====================
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
            LOG_SPEC("onSecondModulo: %d", value);
        }
    }
}



// ================ Minutes ====================

void atMinute(void) {
    // LOG_WARN("atMinute:  %d", value);
    static const int8_t at_array[] = {0, 1, 5, 15, 30, 45};
    for (int i = 0; i < sizeof(at_array); i++) {
        int8_t value = at_array[i];
        if (onTime.atMinute(value)) {
            LOG_WARN("atMinute:  %d", value);
        }
    }
}

void onMinuteModulo(void) {
    static const int8_t array[] = {0, 2, 3, 15, 30, 45, 59};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];

        if (onTime.onMinuteModulo(value)) {
            LOG_SPEC("onMinuteModulo: %d", value);
        }
    }
}





#ifdef ON_TIME_CLASS_SECONDS_VECTOR
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
#else
    void atSecondVector(void) {do {} while (0);}
#endif


#ifdef ON_TIME_CLASS_MINUTES_VECTOR
    const std::vector<uint8_t> minutes = {0, 1, 5, 15, 30, 45};
    const std::vector<uint8_t>& mySecs = minutes;
    void atMinuteVector(void) {
        static bool first_run = true;
        if (first_run) {
            onTime.setAtMinuteFlag(minutes);
            first_run = false;
        }

        for (uint8_t sec : mySecs) {
            if (onTime.getAtMinuteFlag(sec)) {
                LOG_NOTIFY("flagMinute: %d", sec);
            }
        }
    }
#else
    void atMinuteVector(void) {do {} while (0);}
#endif


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


    if (onTime.onSecondModulo(10)) {
        LOG_INFO("!");
    }

    if (onTime.onMinute()) {
        Serial.println();
    }

    // atSecond();
    // onSecondModulo();
    // atSecondVector();

    atMinute();
    onMinuteModulo();
    // atMinuteVector();


    if (first_run) {
        first_run=false;
        LOG_INFO("fine primo loop....\n\n");
    }
    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


