//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 17.11.23
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
    Serial.printf("fonDay:                   %u\n", f.onDay);
    Serial.printf("fonHour:                  %u\n", f.onHour);
    Serial.printf("fonMinute:                %u\n", f.onMinute);
    Serial.printf("fonSecond:                %u\n", f.onSecond);
}


void initFlags() {
    f.ON                         = 1;
    // f.OFF                        = 0;
    f.firstRun                   = 1;
    // f.PressControlTimeExausted   = 0;
    // f.PUMP_ALARM                 = 0;
    f.ascendent                  = 1;
    // f.discendent                 = 0;
    // f.modulo_10_seconds          = 0;
    // f.modulo_30_seconds          = 0;
    // f.modulo_02_minutes          = 0;
    // f.modulo_03_minutes          = 0;
    // f.modulo_05_minutes          = 0;
    // f.modulo_30_minutes          = 0;
    // f.onDay                      = 0;
    // f.onHour                     = 0;
    // f.onMinute                   = 0;
    // f.onSecond                   = 0;
    f.force                      = 1;
    // f.noForce                    = 0;
    f.firstPumpAlarmTime         = 1;
    f.firstPressControlAlarmTime = 1;
    f.waitForPulseEnding         = 1;
    f.withMilliSec               = 1;
}
