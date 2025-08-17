//
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 09.07.22
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    <lnLogger_Class.h>

#include "main.h" // per functions protoype




//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void startButtonHandler(uint8_t pressedLevel) {
    LOG_NOTIFY("[%s] - PRESSED_LEVEL: %d", startButton.pinID(), pressedLevel);
    switch (pressedLevel) {
        case PRESSED_LEVEL_1:
            LOG_INFO("[%s] toggling....", pressControlRelay.pinID());
            pressControlRelay.toggle();
            break;


        // case PRESSED_LEVEL_2:
        //     pressControlRelay.off();
        //     break;


        default:
            LOG_WARN("[%s] - PRESSED_LEVEL Non qualificato", startButton.pinID());
            break;
    }

}



void startButtonNotificationCB(ButtonLongPress_Class *p) {
    static uint32_t lastBeepTime;
    uint32_t phase_beep_duration;

    // LOG_NOTIFY("%s checking...", p->pinID());
    if (p->pressedLevelHasChanged()) {
        LOG_NOTIFY("%s level has been changed", p->pinID());

        phase_beep_duration = 300 * p->currentPressLevel(); // arbitrario....
        switch (p->currentPressLevel()) {
            case PRESSED_LEVEL_1:
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
                waitForPulseEnding(&activeBuzzer, phase_beep_duration);
                break;

            default:
                LOG_INFO("[%s] sono nel default", p->pinID());
                break;
        }
    }

    // --- un BEEP OGNI 2 SECONDI quando si raggiunge il MAX-LEVEL---
    #define ALARM_BEEP_INTERVAL 2000
    if (p->maxLevelReached() ) {
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            activeBuzzer.pulse(1000);
            waitForPulseEnding(&activeBuzzer, 1000);
            LOG_WARN("[%s] ALARM! max pressed level %d reached", p->pinID(), p->currentPressLevel());
            lastBeepTime = millis();
        }
    }
}

