//
// updated by ...: Loreto Notarantonio
// Date .........: 24-08-2025 20.34.06
//

#include <Arduino.h>    // in testa anche per le definizioni dei type




// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
#include    <lnLogger_Class.h>
#include <lnTime_Class.h> // per functions protoype

#include "main.h" // per functions protoype



// ######################################################
// #
// ######################################################
void setTelegramTitle() {
    // myBot.clearMessage();
    // myBot.addFormattedString("<b>pressControl</b> - %s\n", lnTime.nowTime());
    myBot.startNewMessage("<b>PressControl\nTime: %s</b>\n", lnTime.nowTime());
}


// ######################################################
// #
// ######################################################
void sendNtpTelegramMessage() {
    static bool ntpSynchedMessage = false;
    if (lnTime.isNtpSynched()) {
        if (!ntpSynchedMessage) {
            setTelegramTitle();
            myBot.addFormattedString("NTP: <b>Synched</b>\n");
            myBot.send();
            ntpSynchedMessage = true;
        }
    }
    else {
        ntpSynchedMessage = false;
    }
}