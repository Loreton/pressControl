//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 07.49.53
//

#include <Arduino.h>    // in testa anche per le definizioni dei type



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
#include    <lnLogger_Class.h>
#include "main.h" // per functions protoype


void pressControlNotificationCB(ButtonLongPress_Class *p) {
    static uint32_t lastBeepTime;
    uint32_t phase_beep_duration;

    if (p->pressedLevelHasChanged()) {
        phase_beep_duration = 300 * p->currentPressLevel(); // arbitrario....

        switch (p->currentPressLevel()) {
            case PRESSED_LEVEL_1:
                LOG_NOTIFY("%s has been detected ON", p->pinID());
                break;

            case PRESSED_LEVEL_2:
            case PRESSED_LEVEL_3:
            case PRESSED_LEVEL_4:
            case PRESSED_LEVEL_5:
            case PRESSED_LEVEL_6:
            case PRESSED_LEVEL_7:
            case PRESSED_LEVEL_8:
            case PRESSED_LEVEL_9:
                LOG_DEBUG("%s beeping. duration: %lu ms", activeBuzzer->pinID(),  phase_beep_duration);
                activeBuzzer.pulse(phase_beep_duration);
                break;

            default:
                LOG_WARN("[%s] level unknown", p->pinID());
                break;
        }
    }


    // --- un BEEP OGNI 2 SECONDI quando si raggiunge il MAX-LEVEL---
    #define ALARM_BEEP_INTERVAL 2000
    if (p->maxLevelReached() ) {
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            activeBuzzer.pulse(1000);
            LOG_WARN("[%s] ALARM! max pressed level %d reached", p->pinID(), p->currentPressLevel());
            lastBeepTime = millis();
            /**
             * qui prendiamo provvedimenti. chiudiamo prima il relay interno
             * ...e se non funziona chiudiamo il magnetotermico
             * dopo di che se ancora non funziona rimane allarme fisso con il buzzer
            */
            fPressControlTimeExausted = true;
            if (pressControlRelay.isActive()) {
                pressControlRelay.off(); // forziamo il relè
            }
            else {
                magnetoTermicoRelay.startPulse(MAGNETOTERMIC_RELAY_PULSETIME); // significa che togliamo corrente al magnetotermico esterno per 5 secondo
            }

        }
    }

}

