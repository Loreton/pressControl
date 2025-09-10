//
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 18.48.55
//

#include <Arduino.h>    // in testa anche per le definizioni dei type



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <LnTime_Class.h>

#include "functionPrototypes.h" // per functions protoype
#include "main.h"



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
    passiveBuzzer.waitForPulseEnding(3000);
    fPUMP_ALARM = false;
}


// #define BEEP_DURATION_FACTORY



void pumpNotificationCB(ButtonLongPress_Class *p) {
    static bool firstAlarmTime = true;
    static uint32_t lastBeepTime;
    uint32_t phase_beep_duration;
    uint8_t cpLevel = p->currentPressLevel();

    if (p->pressedLevelHasChanged()) {
        phase_beep_duration = cpLevel * BEEP_MULTIPLICATION_FACTOR; // arbitrario....
        switch (cpLevel) {
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
                myBot.addFormattedString("<b>pump level:</b> %d/%d\n<b>duration ms:</b> %lu\n", cpLevel,  p->maxLevels(), phase_beep_duration);
                myBot.send();

                // LOG_INFO("invio dello status su Telegram");
                sendStatusToTelegram(true);
                activeBuzzer.pulse(phase_beep_duration);
                break;

            default:
                LOG_INFO("[%s] sono nel default", p->pinID());
                // fPUMP_ALARM=false;
                break;
        }
    }

    // --- un BEEP OGNI 2 SECONDI quando si raggiunge il MAX-LEVEL---
    #define ALARM_BEEP_INTERVAL 2000
    if (p->maxLevelReached() ) {
        char buffer[16+1];
        const char *levelMS = lnTime.msecToHMS(buffer, 16, p->thresholdLevelValue(cpLevel));

        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            activeBuzzer.pulse(1000); // NON serve per questo pulstante
            lastBeepTime = millis();
            fPUMP_ALARM = true;
        }
        if (f30SecondsModulo || firstAlarmTime) {
            // LOG_WARN("[%s] ALARM! max pressed level %d reached", p->pinID(), cpLevel);
            LOG_WARN("[%s] ALARM! Pump is ON for too much time: %s (level %d reached)", p->pinID(), levelMS, cpLevel);
            setTelegramTitle();
            myBot.addFormattedString("<b>pump ALARM!:</b> max pressed level %d reached", cpLevel);
            myBot.addFormattedString("<b>pump ALARM!:</b>\nPump ON for too much time:\n<b>reached level:</b> %d\n<b>ON time:</b> %s", cpLevel, levelMS);
            myBot.send();
            firstAlarmTime=false;
        }
    }
    else {
        firstAlarmTime=true;
    }


}


