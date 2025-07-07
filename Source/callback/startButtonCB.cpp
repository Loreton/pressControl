//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.41.51
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "lnLogger.h"



#include "ButtonLongPress_Struct.h"
#include "PinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "callBackPrototypes.h" // per functions protoype





extern PinController_Struct activeBuzzer;
PinController_Struct *buzzer = &activeBuzzer;





#define ALARM_BEEP_INTERVAL 2000
void startButtonNotificationHandlerCB(ButtonLongPress_Struct* p) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;

    if (p->m_currentPressLevel != p->m_lastPressedLevel) {
        LOG_INFO("[%s] Pressione in corso (ms:%06ld)", p->m_pinID, (millis() - p->m_pressStartTime));
        switch (p->m_currentPressLevel) {
            case PRESSED_LEVEL_1:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;


            case PRESSED_LEVEL_2:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_3:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_4:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_5:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_6:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_7:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_8:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_9:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            default:
                LOG_INFO("sono nel default: %d", p->m_currentPressLevel);
                break;
        }
        p->m_lastPressedLevel = p->m_currentPressLevel;
    }
    // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
    if (p->m_maxLevelReachedAndNotified ) {
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            buzzer->pulse(1000);
            lastBeepTime = millis();
        }
    }

}


//###########################################################################
//#
//###########################################################################
void startButtonHandlerCB(ButtonLongPress_Struct *p) {
    static bool relayState = false;
    switch (p->m_currentPressLevel) {
        case PRESSED_LEVEL_1:
            LOG_DEBUG("PRESSED_LEVEL_1");
            break;

        case PRESSED_LEVEL_2:
            LOG_DEBUG("PRESSED_LEVEL_2");
            relayState = !relayState;
            if (relayState) {
                // digitalWrite(pressControlRelay_pin, LOW);
                LOG_INFO("  --> Relè ACCESO!");
            } else {
                // digitalWrite(pressControlRelay_pin, HIGH);
                LOG_INFO("  --> Relè SPENTO!");
            }
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

    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->reset();
    // p->m_currentPressLevel = NO_PRESS;
    // p->m_lastPressedLevel = NO_PRESS;
    // p->m_pressDuration = 0;
    // p->m_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.

}
