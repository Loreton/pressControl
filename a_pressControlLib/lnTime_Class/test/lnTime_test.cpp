//
// updated by ...: Loreto Notarantonio
// Date .........: 28-08-2025 16.32.58
//


#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
#include "lnLogger_Class.h"

#include "LnTime_Class.h"


// LnTime_Class lnTime; // Crea un'istanza della classe LnTime

void setup() {
    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    lnTime.setup(2*60); // Chiama il metodo setup della tua istanza di LnTime
}

void loop() {
    // Esempi di utilizzo dei metodi della classe:
    if (lnTime.atSecond()) {
        // LOG_INFO("Current time: %s", lnTime.nowTime());
        // Stampa il tempo completo usando la funzione dedicata
        // struct tm current_time_info = lnTime.getTimeStruct();
        // printLocalTime(&current_time_info); // Chiamata alla funzione esterna
        LOG_INFO("It's a new second!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atMinute()) {
        LOG_WARN("It's a new minute!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atMinute(5)) {
        LOG_WARN("It's a minute 5!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atSecond(14)) {
        LOG_WARN("It's a second 5!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atSecond(44)) {
        LOG_WARN("It's a second 44!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atMinute(12)) {
        LOG_WARN("It's a minute 12!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atMinuteModulo(2)) {
        LOG_WARN("It's a 2 minute modulo!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atMinuteModulo(3)) {
        LOG_WARN("It's a 3 minute modulo!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atSecondModulo(10)) {
        LOG_WARN("It's a 10 second modulo!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }


    if (lnTime.atSecondModulo(15)) {
        LOG_WARN("It's a 15 second modulo!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atSecondModulo(20)) {
        LOG_WARN("It's a 20 second modulo!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    if (lnTime.atSecondModulo(25)) {
        LOG_WARN("It's a 25 second modulo!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }


    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


