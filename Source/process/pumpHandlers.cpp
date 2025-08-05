//
// updated by ...: Loreto Notarantonio
// Date .........: 05-08-2025 08.39.13
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


#include "main.h" // per functions protoype

// extern outPinController_Class activeBuzzer;





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
            pressControlRelay.toggle();

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


}



void pumpNotificationCB(ButtonLongPress_Class *p) {
    static uint32_t lastBeepTime;
    uint32_t phase_beep_duration;


    if (p->pressedLevelHasChanged()) {
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
                LOG_DEBUG("%s beeping. duration: %lu ms", p->pinID(),  phase_beep_duration);
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
            LOG_WARNING("[%s] ALARM! max pressed level %d reached", p->pinID(), p->currentPressLevel());
            lastBeepTime = millis();
        }
    }

}


