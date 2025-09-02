//
// updated by ...: Loreto Notarantonio
// Date .........: 02-09-2025 08.56.12
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
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


// #############################################################
// #
// #############################################################
void loop() {

    uint16_t minute;
    uint16_t second;
    lnTime.update();

    if (lnTime.atSecond()) {
        // {;i("Current time: %s", lnTime.nowTime());
        // Stampa il tempo completo usando la funzione dedicata
        // struct tm current_time_info = lnTime.getTimeStruct();
        // printLocalTime(&current_time_info); // Chiamata alla funzione esterna
        // LOG_INFO("It's a new second!");
        // print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }
    // LOG_INFO("It's a loop!");

    // --- AT
    second=5;
    if (lnTime.atSecond(second)) {
        LOG_WARN("It's at %ld second!", second);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }


    second=12;
    if (lnTime.atSecond(second)) {
        LOG_WARN("It's at %ld second!", second);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }



    // --- MODULO
    second=5;
    if (lnTime.onSecondModulo(second)) {
        LOG_WARN("It's on %ld second modulo!", second);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    second=10;
    if (lnTime.onSecondModulo(second)) {
        LOG_WARN("It's on %ld second modulo!", second);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    second=15;
    if (lnTime.onSecondModulo(second)) {
        LOG_WARN("It's on %ld second modulo!", second);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    second=20;
    if (lnTime.onSecondModulo(second)) {
        LOG_WARN("It's on %ld second modulo!", second);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }



    // --- AT
    minute=1;
    if (lnTime.atMinute(minute)) {
        LOG_WARN("It's at %ld minute!", minute);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    minute=3;
    if (lnTime.atMinute(minute)) {
        LOG_WARN("It's at %ld minute!", minute);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    minute=5;
    if (lnTime.atMinute(minute)) {
        LOG_WARN("It's at %ld minute!", minute);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }


    // --- MODULO
    minute=2;
    if (lnTime.onMinuteModulo(minute)) {
        LOG_WARN("It's on %ld minute modulo!", minute);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    minute=4;
    if (lnTime.onMinuteModulo(minute)) {
        LOG_WARN("It's on %ld minute modulo!", minute);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    minute=6;
    if (lnTime.onMinuteModulo(minute)) {
        LOG_WARN("It's on %ld minute modulo!", minute);
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }


    if (first_run) {
        first_run=false;
        LOG_INFO("fine primo loop....\n\n");
    }
    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


