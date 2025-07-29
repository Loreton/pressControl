//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 08.25.44
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger_Class.h" // printf:XFN()
#include "lnGlobalVars.h" //



#include "outPinController_Class.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
// #include "callBackPrototypes.h" // per functions protoype


extern outPinController_Class activeBuzzer;
// outPinController_Class *buzzer2 = &activeBuzzer;

        // #ifdef __BUTTONLONGPRESS_CLASS__
#include "ButtonLongPress_Class.h"


void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration) {
    LOG_NOTIFY("%s beeping. duration: %lu ms", p->m_pinID,  beep_duration);
    activeBuzzer.pulse(beep_duration);
}

// #include "ButtonLongPress_Struct.h"


// void beepNotification(ButtonLongPress_Struct *p, uint32_t beep_duration) {
//     LOG_NOTIFY("%s beeping. duration: %lu ms", p->m_pinID,  beep_duration);
//     activeBuzzer.pulse(beep_duration);
// }

// #endif



