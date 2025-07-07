//
// updated by ...: Loreto Notarantonio
// Date .........: 06-07-2025 20.51.59
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_99
#include "lnLogger.h"
// #include "@debouncedButton_sClass.h"

// ---------------------------------
// - project headers files
// ---------------------------------
#include "main.h"


//###########################################################################
//#
//###########################################################################
void startButton_Action(ButtonLongPress_Struct *p) {
    // LOG_DEBUG("sono qui p->m_currentPressLevel %d", p->m_currentPressLevel);

    // switch (p->m_currentPressLevel) {
    //     case PRESSED_LEVEL_1:
    //         LOG_DEBUG("PRESSED_LEVEL_1");
    //         break;

    //     case PRESSED_LEVEL_2:
    //         LOG_DEBUG("PRESSED_LEVEL_2");
    //         relayState = !relayState;
    //         if (relayState) {
    //             digitalWrite(pressControlRelay_pin, LOW);
    //             LOG_INFO("  --> Relè ACCESO!");
    //         } else {
    //             digitalWrite(pressControlRelay_pin, HIGH);
    //             LOG_INFO("  --> Relè SPENTO!");
    //         }
    //         break;

    //     case PRESSED_LEVEL_3:
    //         LOG_DEBUG("PRESSED_LEVEL_3");
    //         break;

    //     case PRESSED_LEVEL_4:
    //         LOG_DEBUG("PRESSED_LEVEL_4");
    //         break;

    //     default:
    //         LOG_DEBUG("Sconosciuto/Non Qualificato");
    //         break;
    // }
}
