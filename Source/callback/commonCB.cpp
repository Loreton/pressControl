//
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 13.58.39
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "lnLogger.h" // printf:XFN()
#include "lnGlobalVars.h" //



#include "ButtonLongPress_Struct.h"
#include "LedController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
// #include "callBackPrototypes.h" // per functions protoype


extern LedController_Struct activeBuzzer;
// LedController_Struct *buzzer2 = &activeBuzzer;


void beepNotification(ButtonLongPress_Struct *p, uint16_t beep_duration) {
    LOG_TRACE("[%s] beeping", p->m_pinID);
    activeBuzzer.pulse(beep_duration);
}




