//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 08.47.39
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// #include "lnLogger_Struct.h"
#include "main.h" // per functions protoype








//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void startButtonHandler(uint8_t pressedLevel) {
    LOG_NOTIFY("PRESSED_LEVEL: %d", pressedLevel);
    switch (pressedLevel) {
        case PRESSED_LEVEL_1:
            pressControlRelay.toggle();
            break;

        // case PRESSED_LEVEL_2:
        //     pressControlRelay.off();
        //     break;


        default:
            LOG_WARNING("PRESSED_LEVEL Non Qualificato");
            break;
    }

}

