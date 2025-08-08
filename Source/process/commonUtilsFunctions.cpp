//
// updated by ...: Loreto Notarantonio
// Date .........: 08-08-2025 11.25.34
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "main.h"




void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration) {
    LOG_NOTIFY("%s beeping. duration: %lu ms", p->m_pinID,  beep_duration);
    activeBuzzer.pulse(beep_duration);
}



void waitForPulseEnding(outPinController_Class *p, int32_t timeOut) {
    while (p->isPlayingSomething() && timeOut > 0) {
        delay(10);
        timeOut -= 10;
        p->update();
        LOG_TRACE("\t[%s] waiting", p->pinID());
    }
}

void waitForPulseEnding(PassiveBuzzer_Class *p, int32_t timeOut) {
    while (p->isPlayingSomething() && timeOut > 0) {
        delay(10);
        timeOut -= 10;
        p->update();
        LOG_TRACE("\t[%s] waiting", p->pinID());
    }
}

