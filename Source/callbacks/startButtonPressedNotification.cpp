//
// updated by ...: Loreto Notarantonio
// Date .........: 23-07-2025 17.01.32
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


#include "lnLogger.h"


#include "callBackPrototypes.h" // per functions protoype
#include "main.h" // per functions protoype

extern LedController_Struct activeBuzzer;



void startButtonNotificationCB(ButtonLongPress_Struct *p) {
    static uint32_t lastBeepTime;
    uint32_t next_interval;
    uint32_t phase_beep_duration;
    // uint32_t elapsed = millis() - p->m_pressStartTime;
    const char *tabs="\t\t\t\t\t\t\t\t";


    if (p->m_currentPressLevel != p->m_lastPressedLevel) {
        next_interval = p->m_gapThresholds[p->m_currentPressLevel];

        // elapsed = millis() - p->m_pressStartTime;
        phase_beep_duration = 300 * p->m_currentPressLevel; // arbitrario....


        LOG_INFO("[%s]:", p->m_pinID);
        LOG_INFO("\tPRESSED_LEVEL  %2d/%2d", p->m_currentPressLevel, p->m_numThresholds);
        LOG_NOTIFY("\telapsed ms:    %lu", p->m_elapsed);
        LOG_NOTIFY("\tnext_interval: %lu", next_interval);
        LOG_NOTIFY("\tbeep_duration: %lu", phase_beep_duration);

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
            // activeBuzzer.pulse(1000); // NON serve per questo pulstante
            LOG_WARNING("[%s] ALARM! max pressed level %d reached", p->m_pinID, p->m_currentPressLevel);
            lastBeepTime = millis();
        }
    }

}