//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 14.28.47
//

//--- flags.cpp

#include "Arduino.h"
#include "flags.h"

// Stampa tutti i flag e il loro valore
void printFlags() {
    Serial.printf("ON:                       %u\n", f.ON);
    Serial.printf("OFF:                      %u\n", f.OFF);
    Serial.printf("firstRun:                 %u\n", f.firstRun);
    Serial.printf("PressControlTimeExausted: %u\n", f.PressControlTimeExausted);
    Serial.printf("PUMP_ALARM:               %u\n", f.PUMP_ALARM);
    Serial.printf("ascendent:                %u\n", f.ascendent);
    Serial.printf("discendent:               %u\n", f.discendent);
    Serial.printf("modulo_10_seconds:        %u\n", f.modulo_10_seconds);
    Serial.printf("modulo_30_seconds:        %u\n", f.modulo_30_seconds);
    Serial.printf("modulo_02_minutes:        %u\n", f.modulo_02_minutes);
    Serial.printf("modulo_03_minutes:        %u\n", f.modulo_03_minutes);
    Serial.printf("modulo_05_minutes:        %u\n", f.modulo_05_minutes);
    Serial.printf("modulo_30_minutes:        %u\n", f.modulo_30_minutes);
    Serial.printf("fonDay:                   %u\n", f.fonDay);
    Serial.printf("fonHour:                  %u\n", f.fonHour);
    Serial.printf("fonMinute:                %u\n", f.fonMinute);
    Serial.printf("fonSecond:                %u\n", f.fonSecond);
}


void initFlags() {
    f.ON       = 1;
    f.firstRun = 1;
}
