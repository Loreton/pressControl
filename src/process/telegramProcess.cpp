//
// updated by ...: Loreto Notarantonio
// Date .........: 11-09-2025 07.02.50
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
    myBot.startNewMessage("<b>ESP32-PressControl\nTime:</b> %s\n", lnTime.now());
}


