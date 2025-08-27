//
// updated by ...: Loreto Notarantonio
// Date .........: 27-08-2025 14.12.48
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
#include    <lnLogger_Class.h>
#include <LnTime_Class.h>


#include "main.h" // per functions protoype



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



// ##############################################################
// #  prepara il titolo con la data e poi aggiunge messaggio
// ##############################################################
void sendTelegram(const char *message) {
    // const char *title = "pressControl";
    // const char *parseMode="HTML";
    // char timeStr[16];
    // lnTime.timeStamp(timeStr, sizeof(timeStr)); // timeStamp

    // snprintf(msgBuffer, sizeof(msgBuffer), "<b>pressControl</b> - %s%%0A%s", timeStr, message);
    // len = snprintf(tgMsg.bufferData, tgMsg.MAX_DATA_LEN, "&parse_mode=%s&text=%s", parseMode, message);
    // len = snprintf(tgMsg.bufferData, tgMsg.MAX_DATA_LEN, "&parse_mode=%s<b>%s</b> - %s%%0A%s", parseMode, title, timeStr, message);
    // sendMessageToTelegram();

}

