//
// updated by ...: Loreto Notarantonio
// Date .........: 01-09-2025 15.17.24
//

#include <Arduino.h>    // in testa anche per le definizioni dei type



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <LnTime_Class.h>

#include "main.h" // per functions protoype



//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void pumpHandler(ButtonLongPress_Class *p) {
    switch (p->currentPressLevel()) {
        case PRESSED_LEVEL_1:
            LOG_DEBUG("PRESSED_LEVEL_1");
            break;

        case PRESSED_LEVEL_2:
            LOG_DEBUG("PRESSED_LEVEL_2");
            break;

        case PRESSED_LEVEL_3:
            LOG_DEBUG("PRESSED_LEVEL_3");
            break;

        case PRESSED_LEVEL_4:
            LOG_DEBUG("PRESSED_LEVEL_4");
            break;

        default:
            LOG_DEBUG("Sconosciuto/Non Qualificato");
            break;
    }


    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->reset();
    passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 150, fDiscendent); // Scala ascendente, 150ms per nota)
    waitForPulseEnding(&passiveBuzzer, 3000);
    fPUMP_ALARM = false;

}


#define BEEP_DURATION_FACTORY



void pumpNotificationCB(ButtonLongPress_Class *p) {
    static bool firstAlarmTime = true;
    static uint32_t lastBeepTime;
    uint32_t phase_beep_duration;


    if (p->pressedLevelHasChanged()) {
        phase_beep_duration = p->currentPressLevel() * BEEP_DURATION_FACTORY; // arbitrario....
        switch (p->currentPressLevel()) {
            case PRESSED_LEVEL_1:
                LOG_NOTIFY("%s has been detected ON", p->pinID());
                passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 150, fAscendent); // Scala ascendente, 150ms per nota)
                break;

            case PRESSED_LEVEL_2:
            case PRESSED_LEVEL_3:
            case PRESSED_LEVEL_4:
            case PRESSED_LEVEL_5:
            case PRESSED_LEVEL_6:
            case PRESSED_LEVEL_7:
            case PRESSED_LEVEL_8:
            case PRESSED_LEVEL_9:

                LOG_INFO("%s beeping. duration: %lu ms", p->pinID(),  phase_beep_duration);
                setTelegramTitle();
                myBot.addFormattedString("<b>pump level:</b> %d/%d\n<b>duration ms:</b> %lu\n", p->currentPressLevel(),  p->maxLevels(), phase_beep_duration);
                myBot.send();

                activeBuzzer.pulse(phase_beep_duration);
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
            activeBuzzer.pulse(1000); // NON serve per questo pulstante
            lastBeepTime = millis();
            fPUMP_ALARM = true;
        }
        if (fModulo30Seconds || firstAlarmTime) {
            LOG_WARN("[%s] ALARM! max pressed level %d reached", p->pinID(), p->currentPressLevel());
            setTelegramTitle();
            myBot.addFormattedString("<b>pump ALARM!:</b> max pressed level %d reached", p->currentPressLevel());
            myBot.send();
            firstAlarmTime=false;
        }
    }
    else {
        firstAlarmTime=true;
    }


}


