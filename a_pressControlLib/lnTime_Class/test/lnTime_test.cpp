//
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 12.12.04
//


// #define  log_module_level log_default_trace
#include "lnLogger_Class.h"

#include "LnTime_Class.h"


// LnTime_Class lnTime; // Crea un'istanza della classe LnTime

void setup() {
    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    lnTime.setup(2*60); // Chiama il metodo setup della tua istanza di LnTime
    Serial.println("Sono lnTime TEST");
}



bool    first_run=true;


void Seconds(void) {
    static const int8_t array[] = {0, 15, 30, 45, 120};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];

        if (lnTime.atSecond(value)) {
            log_warn("Second at %d", value);
        }

        if (lnTime.onSecondModulo(value)) {
            log_spec("Second on %d modulo!", value);
        }
    }
}


void Minutes(void) {
    static const int8_t array[] = {0, 15, 30, 45, 60};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];
        if (lnTime.atMinute(value)) {
            log_warn("Minute at %d", value);
        }

        if (lnTime.onMinuteModulo(value)) {
            log_spec("Minute on %d modulo!", value);
        }
    }
}







// #############################################################
// #
// #############################################################
int8_t counter=0;
void loop() {
    uint16_t value;
    uint16_t minute;
    uint16_t second;
    lnTime.update();

    // int8_t onSeconds[] = {0, 15, 30, 45};
    // int8_t atMinutes[] = {0, 15, 30, 45};
    // int8_t onMinutes[] = {0, 15, 30, 45};



    if (lnTime.onSecond()) {
        // {;i("Current time: %s", lnTime.nowTime());
        // Stampa il tempo completo usando la funzione dedicata
        // struct tm current_time_info = lnTime.getTimeStruct();
        // printLocalTime(&current_time_info); // Chiamata alla funzione esterna
        // log_info("It's a new second!");
        // print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }
    if (lnTime.onMinute()) {
        log_info("It's a new minute!");
        // print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.onHour()) {
        log_info("It's a new hour!");
        // print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    Seconds();
    Minutes();



    if (first_run) {
        first_run=false;
        log_info("fine primo loop....\n\n");
    }
    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


