//
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 09.43.30
//

#include <Arduino.h>    // in testa anche per le definizioni dei type




// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#define  LOG_MODULE_LEVEL LOG_DEFAULT_LEVEL
#include    <lnLogger_Class.h>
#include <LnTime_Class.h> // per functions protoype

#include "main.h" // per functions protoype



// ######################################################
// #
// ######################################################
void setTelegramTitle() {
    // myBot.startNewMessage("<b>ESP32-PressControl\nTime:</b> %s\n", lnTime.now());
    return;
}


// ######################################################
// #
// ######################################################
// void sendNtpSynchedTelegramMessage_OK() {
//     static bool ntpSynchedMessage = false;
//     if (lnTime.isNtpSynched()) {
//         if (!ntpSynchedMessage) {
//             setTelegramTitle();
//             myBot.addFormattedString("<b>NTP:</b> Synched\n");
//             myBot.send();
//             ntpSynchedMessage = true;
//         }
//     }
//     else {
//         ntpSynchedMessage = false;
//     }
// }// ######################################################
// #
// ######################################################
// void sendNtpSynchedTelegramMessage() {
//     static bool ntpSynchedMessage = false;
//     // if (lnTime.isNtpSynched()) {
//         // if (!ntpSynchedMessage) {
//             setTelegramTitle();
//             myBot.addFormattedString("<b>NTP:</b> Synched\n");
//             myBot.send();
//             ntpSynchedMessage = true;
//         // }
//     // }
//     // else {
//         // ntpSynchedMessage = false;
//     // }
// }