//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 17.08.47
//

#pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type

    #include "ButtonLongPress_Struct.h"

    void pressControlNotificationCB(ButtonLongPress_Struct* p);
    void startButtonNotificationCB(ButtonLongPress_Struct* p);
    void pumpPressedNotificationCB(ButtonLongPress_Struct* p);
    void beepNotification(ButtonLongPress_Struct *p, uint32_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Struct *p);

