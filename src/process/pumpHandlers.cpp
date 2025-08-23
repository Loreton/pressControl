//
// updated by ...: Loreto Notarantonio
// Date .........: 23-08-2025 17.40.44
//

#include <Arduino.h>    // in testa anche per le definizioni dei type



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include <lnLogger_Class.h>
#include <lnTime_Class.h>

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
            // pressControlRelay.toggle();

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



void pumpNotificationCB(ButtonLongPress_Class *p) {
    static uint32_t lastBeepTime;
    uint32_t phase_beep_duration;


    if (p->pressedLevelHasChanged()) {
        // phase_beep_duration = 300 * p->currentPressLevel() / 30; // arbitrario....
        phase_beep_duration = p->currentPressLevel() * 3 * 1000; // arbitrario....
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
                myBot.addFormattedString("pump level: <b>%d</b>\nduration ms: <b>%lu</b>\n", p->currentPressLevel(), phase_beep_duration);
                myBot.send();

                activeBuzzer.pulse(phase_beep_duration);
                // char dateStr[16];
                // lnTime.timeStamp(dateStr, sizeof(dateStr));
                // snprintf(tgMessageBuffer, TG_MSG_MAX_SIZE, "<b>ESP32</b> - %s%%aPump is stiil ON%%alevel: <b>%d/5d</b>",
                //           dateStr, p->currentPressLevel(), p->maxLevels());
                // sendMessageToTelegram(tgMessageBuffer, modeHTML);

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
            LOG_WARN("[%s] ALARM! max pressed level %d reached", p->pinID(), p->currentPressLevel());
            lastBeepTime = millis();
            fPUMP_ALARM = true;
        }
    }

}


