//
// updated by ...: Loreto Notarantonio
// Date .........: 06-06-2025 16.24.48
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_1
#define LOG_LEVEL_2x
#include "@logMacros.h"
#include "@pinOperations.h"
#include "@lnString.h"
#include "@ln_MillisTimer.h"


// ---------------------------------
// - project headers files
// ---------------------------------
#include "@a_mainProject.h"
#include "@pinDefinitions.h"
#include "@readPressedButton.h"
#include "@pressControl.h"




extern    const int8_t PUMPSTATE_OVERFLOWED_LEVEL;


// extern io_input_pin_struct_t   *pumpState;

// =====================================
// = process lo stato della pompa
// = ON:
// =   tono crescente in frequenza (in partenza)
// =   in base al tempo in cui rimane accesa emettiamo un suono
// =   sempre più prolungato fino allo spegnimento del relè del pressControl
// = OFF:
// =    tono calante in frequenza
// =====================================
bool pumpStateForAlarm(void) {
    static uint32_t activeBuzzerDuration;
    static int8_t last_level = -1; // per capire se il nuovo livello è cambiato rispetto al precedente
    static bool first_run=true;

    uint8_t pressed_level = readLongPressPin(pumpState);


    // --- check timer
    if ( (pinTimerExpired(pumpState)) || (pumpState->is_OFF) ) {
        printf2_NFN("%s timer expired\n", pumpState->pinID);
        pinTimerStop(pumpState);
        activeBuzzerOFF();
    }



    // --- solo nel caso di pressing del tasto e di cambio livello
    if ( (pumpState->is_ON) && (last_level != pumpState->pressedLevel) ) {
        last_level = pumpState->pressedLevel;

        if (pumpState->pressedLevel < 1)
            activeBuzzerDuration = 0;
        else if (pumpState->pressedLevel > 1)
            activeBuzzerDuration = pumpState->pressedLevel*1000;


        // printf1_NFN("[%-15s.%02d]: pressed level: %d/%d\n", pumpState->name, pumpState->pin, pumpState->pressedLevel, pumpState->n_thresholds-2);
        printf1_NFN("%s pressed level: %d/%d\n", pumpState->pinID, pumpState->pressedLevel, pumpState->n_thresholds-1);
        switch (pumpState->pressedLevel) {

            case PRESSED_TIME_01:
                printf1_NFN("%s buzzerScaleUp\n", pumpState->pinID);
                pressControlRelayStatus(PUMP_ON);
                passiveBuzzerScaleUp();
                activeBuzzerDuration = 0; // no other beep
                break;

            case PRESSED_TIME_02:
            case PRESSED_TIME_04:
            case PRESSED_TIME_05:
                break;

            case PRESSED_TIME_06:
                printf1_NFN("%s pressed PRESSED_TIME_06: %d\n",pumpState->pinID, pumpState->pressedLevel);
                printf1_NFN("%s turning OFF pressControl\n", pumpState->pinID);

                pumpState->isReleased=false;
                pumpState->pressedLevel=NO_BUTTON_PRESSED;
                pinTimerStop(pumpState);
                pinOFF(pumpLED);

                activeBuzzerOFF();
                pressControlRelayOFF(PUMP_FORCED_OFF);  // force pressControl OFF
                break;


        } // end switch

        if (activeBuzzerDuration != 0 && !pumpState->timer.enabled) {
            pressControlRelayStatus(PUMP_STILL_ON);
            pinTimerStart(pumpState, activeBuzzerDuration);
            activeBuzzerON();
            printf2_NFN("%s buzzer ON\n", pumpState->pinID);
        }
    }
    else if (pumpState->isReleased) {
        printf1_NFN("%s pump is released\n", pumpState->pinID);
        activeBuzzerOFF();
        pinTimerStop(pumpState);
        // --- OFF alarms
        pinOFF(pumpHornAlarmRelay);
        passiveBuzzerScaleDown();
        pinOFF(pumpLED);

        // check status
        pressControlRelayStatus(PUMP_OFF);
    }

    return 0;
} // end function
