//
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 09.07.33
//

#include <Arduino.h>    // in testa anche per le definizioni dei type




// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include    <lnLogger_Class.h>
#include <lnTime_Class.h> // per functions protoype

#include "main.h" // per functions protoype



void setTelegramTitle() {
    myBot.clearMessage();
    myBot.addFormattedString("<b>pressControl</b> - %s\n", lnTime.nowTime());
}
