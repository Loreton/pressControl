//
// updated by ...: Loreto Notarantonio
// Date .........: 09-09-2025 11.39.31
//

#include <Arduino.h>    // in testa anche per le definizioni dei type



// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include    <lnLogger_Class.h>
#include "main.h" // per functions protoype




//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void pressControlHandler(ButtonLongPress_Class *p) {
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
    // passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 150, fDiscendent); // Scala ascendente, 150ms per nota)
    // waitForPulseEnding(&passiveBuzzer, 3000);
    // fPUMP_ALARM = false;
}




void pressControlNotificationCB(ButtonLongPress_Class *p) {
    static uint32_t lastBeepTime;
    static bool firstAlarmTime = true;
    uint32_t phase_beep_duration;
    uint8_t currentPressedLevel = p->currentPressLevel();

    if (p->pressedLevelHasChanged()) {
        phase_beep_duration = 300 * currentPressedLevel; // arbitrario....

        switch (currentPressedLevel) {
            case PRESSED_LEVEL_1:
                LOG_NOTIFY("%s has been detected ON", p->pinID());
                activeBuzzer.blinking(300, 200, 3);
                activeBuzzer.waitForPulseEnding(2000);
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
    #define TIME_BUFFER_LENGTH 16
    if (p->maxLevelReached() ) {
        uint32_t currentPressedTime = p->thresholdLevelValue(currentPressedLevel);
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            activeBuzzer.pulse(1000);
            // LOG_WARN("[%s] ALARM! max pressed level %d reached", p->pinID(), currentPressedLevel);
            LOG_WARN("[%s] ALARM! max pressing level %d (time %lu) has been reached", p->pinID(), currentPressedLevel, currentPressedTime);
            lastBeepTime = millis();

            if (f30SecondsModulo || firstAlarmTime) {
                LOG_WARN("[%s] ALARM! max pressing levela %d (time %lu) has been reached", p->pinID(), currentPressedLevel, currentPressedTime);
                setTelegramTitle();
                char buffer[TIME_BUFFER_LENGTH+1];
                myBot.addFormattedString("<b>PressControl ALARM!:</b>\n\t pressing time %s expired", lnTime.msecToHMS(buffer, TIME_BUFFER_LENGTH, currentPressedTime) );
                myBot.send();
                firstAlarmTime=false;
            }


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
    else {
        firstAlarmTime=true;
    }


}

