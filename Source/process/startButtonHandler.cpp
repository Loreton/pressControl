//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 14.24.27
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// #include "lnLogger_Struct.h"
#include "main.h" // per functions protoype








//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void startButtonHandler(uint8_t pressedLevel) {
    LOG_NOTIFY("[%s] - PRESSED_LEVEL: %d", startButton.pinID(), pressedLevel);
    switch (pressedLevel) {
        case PRESSED_LEVEL_1:
            LOG_INFO("[%s] toggle...ing", pressControlRelay.pinID());
            pressControlRelay.toggle();
            break;


        // case PRESSED_LEVEL_2:
        //     pressControlRelay.off();
        //     break;


        default:
            LOG_WARNING("[%s] - PRESSED_LEVEL Non qualificato", startButton.pinID());
            break;
    }

}

