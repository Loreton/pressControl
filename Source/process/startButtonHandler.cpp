//
// updated by ...: Loreto Notarantonio
// Date .........: 23-07-2025 16.31.00
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


#include "lnLogger.h"
#include "main.h" // per functions protoype








//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void startButtonHandler(uint8_t pressedLevel) {
    static bool relayState = false;
    switch (pressedLevel) {
        case PRESSED_LEVEL_1:
            LOG_DEBUG("PRESSED_LEVEL: 1");
            pressControlRelay.toggle();
            break;

        // case PRESSED_LEVEL_2:
        //     LOG_DEBUG("PRESSED_LEVEL: 2");
        //     break;


        default:
            LOG_DEBUG("PRESSED_LEVEL Non Qualificato");
            break;
    }

}


// //###########################################################################
// //# richiamata quando il pulsante viene rilasciato
// //###########################################################################
// void startButtonHandler(ButtonLongPress_Struct *p) {
//     static bool relayState = false;
//     switch (p->m_currentPressLevel) {
//         case PRESSED_LEVEL_1:
//             LOG_DEBUG("PRESSED_LEVEL: 1");
//             break;

//         case PRESSED_LEVEL_2:
//             LOG_DEBUG("PRESSED_LEVEL: 2");
//             pressControlRelay.toggle();

//             break;

//         case PRESSED_LEVEL_3:
//             LOG_DEBUG("PRESSED_LEVEL: 3");
//             break;

//         case PRESSED_LEVEL_4:
//             LOG_DEBUG("PRESSED_LEVEL: 4");
//             break;

//         default:
//             LOG_DEBUG("PRESSED_LEVEL Non Qualificato");
//             break;
//     }

// }
