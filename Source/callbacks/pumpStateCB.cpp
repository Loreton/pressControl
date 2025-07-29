//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 07.58.53
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// #include "lnLogger_Struct.h"


// #include "callBackPrototypes.h" // per functions protoype
#include "main.h" // per functions protoype


        #ifdef __BUTTONLONGPRESS_CLASS__

//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void pumpStateHandlerCB(ButtonLongPress_Class *p) {
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

    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->reset();
    // p->m_currentPressLevel = NO_PRESS;
    // p->m_lastPressedLevel = NO_PRESS;
    // p->m_pressDuration = 0;
    // p->m_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.

}

#else

//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void pumpStateHandlerCB(ButtonLongPress_Struct *p) {
    static bool relayState = false;
    switch (p->m_currentPressLevel) {
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

    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->reset();
    // p->m_currentPressLevel = NO_PRESS;
    // p->m_lastPressedLevel = NO_PRESS;
    // p->m_pressDuration = 0;
    // p->m_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.

}


#endif