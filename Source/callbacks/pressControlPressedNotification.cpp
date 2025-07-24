//
// updated by ...: Loreto Notarantonio
// Date .........: 23-07-2025 19.15.47
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


#include "lnLogger.h"


#include "callBackPrototypes.h" // per functions protoype
#include "main.h" // per functions protoype

extern LedController_Struct activeBuzzer;



void pressControlNotificationCB(ButtonLongPress_Struct *p) {
    static uint32_t lastBeepTime;
    uint32_t next_interval;
    uint32_t phase_beep_duration;
    // uint32_t elapsed = millis() - p->m_pressStartTime;


    if (p->m_currentPressLevel != p->m_lastPressedLevel) {
        next_interval = p->m_gapThresholds[p->m_currentPressLevel];

        // elapsed = millis() - p->m_pressStartTime;
        phase_beep_duration = 1000 * p->m_currentPressLevel; // arbitrario....

        LOG_NOTIFY("[%s] PRESSED_LEVEL %d/%d - elapsed ms:%6lu - next_interval: %lu",
                        p->m_pinID,
                        p->m_currentPressLevel,
                        p->m_numThresholds,
                        p->m_elapsed,
                        next_interval);

        switch (p->m_currentPressLevel) {
            case PRESSED_LEVEL_1:
            case PRESSED_LEVEL_2:
            case PRESSED_LEVEL_3:
            case PRESSED_LEVEL_4:
            case PRESSED_LEVEL_5:
            case PRESSED_LEVEL_6:
            case PRESSED_LEVEL_7:
            case PRESSED_LEVEL_8:
            case PRESSED_LEVEL_9:
                LOG_NOTIFY("%s beeping. duration: %lu ms", p->m_pinID,  phase_beep_duration);
                activeBuzzer.pulse(phase_beep_duration);
                break;

            default:
                LOG_INFO("[%s] sono nel default", p->m_pinID);
                break;
        }

        p->m_lastPressedLevel = p->m_currentPressLevel;
    }
    // --- un BEEP OGNI 2 SECONDI quando si raggiunge il MAX-LEVEL---
    #define ALARM_BEEP_INTERVAL 2000
    if (p->m_maxLevelReachedAndNotified ) {
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            activeBuzzer.pulse(1000);
            LOG_WARNING("[%s] ALARM! max pressed level %d reached", p->m_pinID, p->m_currentPressLevel);
            lastBeepTime = millis();
            if (pressControlRelay.isActive()) {
                pressControlRelay.off(); // forziamo il relè
            }
            else {
                magnetoTermicoRelay.startPulse(1000); // significa che togliamo corrente al magnetotermico esterno per 1 secondo
            }
        }
    }

}