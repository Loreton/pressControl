//
// updated by ...: Loreto Notarantonio
// Date .........: 15-09-2025 15.20.36
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <LnTime_Class.h>


#include "main.h" // per functions protoype



void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration) {
    LOG_NOTIFY("%s beeping. duration: %lu ms", p->m_pinID,  beep_duration);
    activeBuzzer.pulse(beep_duration);
}


void sirenaBitonale(void) {
    uint32_t duration[]   = {1000, 167, 167, 167, 1000, 167, 167, 167, 1000 };
    uint16_t tone[]       = { 392, 660, 392, 660,  392, 660, 392, 660, 392 };
    uint8_t nTones = sizeof(tone) / sizeof(tone[0]);

    for (uint8_t index=0; index < nTones; index++) {
        LOG_SPEC("[%d/%d] - freq: %7lu  duration: %lu", index, nTones, tone[index], duration[index] );
        passiveBuzzer.playFixedTone(tone[index], duration[index], true);
    }
}



// #############################################################
// # il timeout non è vincolante, se il suono finisce prima si esce...
// #############################################################
// void waitForPulseEnding(outPinController_Class *p, int32_t timeOut) {
//     while (p->isPlayingSomething() && timeOut > 0) {
//         delay(10);
//         timeOut -= 10;
//         p->update();
//         LOG_TRACE("\t[%s] waiting for the end of pulse", p->pinID());
//     }
// }

// void waitForPulseEnding(PassiveBuzzer_Class *p, int32_t timeOut) {
//     while (p->isPlayingSomething() && timeOut > 0) {
//         delay(10);
//         timeOut -= 10;
//         p->update();
//         LOG_TRACE("\t[%s] waiting", p->pinID());
//     }
// }



// ##############################################################
// #  prepara il titolo con la data e poi aggiunge messaggio
// ##############################################################
// void sendTelegram(const char *message) {
    // const char *title = "pressControl";
    // const char *parseMode="HTML";
    // char timeStr[16];
    // lnTime.timeStamp(timeStr, sizeof(timeStr)); // timeStamp

    // snprintf(msgBuffer, sizeof(msgBuffer), "<b>pressControl</b> - %s%%0A%s", timeStr, message);
    // len = snprintf(tgMsg.bufferData, tgMsg.MAX_DATA_LEN, "&parse_mode=%s&text=%s", parseMode, message);
    // len = snprintf(tgMsg.bufferData, tgMsg.MAX_DATA_LEN, "&parse_mode=%s<b>%s</b> - %s%%0A%s", parseMode, title, timeStr, message);
    // sendMessageToTelegram();

// }

