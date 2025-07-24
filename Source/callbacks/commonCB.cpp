//
// updated by ...: Loreto Notarantonio
// Date .........: 23-07-2025 07.49.53
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger.h" // printf:XFN()
#include "lnGlobalVars.h" //



#include "ButtonLongPress_Struct.h"
#include "LedController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
// #include "callBackPrototypes.h" // per functions protoype


extern LedController_Struct activeBuzzer;
// LedController_Struct *buzzer2 = &activeBuzzer;


void beepNotification(ButtonLongPress_Struct *p, uint32_t beep_duration) {
    LOG_NOTIFY("%s beeping. duration: %lu ms", p->m_pinID,  beep_duration);
    activeBuzzer.pulse(beep_duration);
}




