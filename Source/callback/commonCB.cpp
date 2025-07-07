//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.40.59
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "lnLogger.h" // printf:XFN()
#include "lnGlobalVars.h" //



#include "ButtonLongPress_Struct.h"
#include "PinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
// #include "callBackPrototypes.h" // per functions protoype


extern PinController_Struct activeBuzzer;
// PinController_Struct *buzzer2 = &activeBuzzer;


void beepNotification(ButtonLongPress_Struct *p, uint16_t beep_duration) {
    LOG_TRACE("[%s] beeping", p->m_pinID);
    activeBuzzer.pulse(beep_duration);
}

//###########################################################################
//#
//###########################################################################
void showStatusCB(ButtonLongPress_Struct *p) {
    // printf0_FN("\t%-18s: %2d - (%d)" , "pin nr"         , p->pin                   , p->mode);
    LOG_DEBUG("%s", p->m_pinID);
    LOG_DEBUG("\t%-18s: %2d",   "pressedLogicLevel",          p->m_pressedLogicLevel); //,           str_pinLevel[p->m_pressedLogicLevel]);
    LOG_DEBUG("\t%-18s: %2d",   "buttonPressed",              p->m_buttonPressed); //,               str_TrueFalse[p->m_buttonPressed]);
    LOG_DEBUG("\t%-18s: %2d",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified); //,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    LOG_DEBUG("\t%-18s: %2d - %s",   "pressedLogicLevel",          p->m_pressedLogicLevel,           str_pinLevel[p->m_pressedLogicLevel]);
    LOG_DEBUG("\t%-18s: %2d - %s",   "buttonPressed",              p->m_buttonPressed,               str_TrueFalse[p->m_buttonPressed]);
    LOG_DEBUG("\t%-18s: %2d - %s",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    LOG_DEBUG("\t%-18s: nr:%2d --> [ ", "threshold"      , p->m_numThresholds);
    for (int8_t j=0; j < p->m_numThresholds; j++) {
        LOG_DEBUG("%2d, ", p->m_pressThresholds[j]);
    }
    LOG_DEBUG(" ]"); // close threshold line

    LOG_DEBUG(""); // blank line

}




