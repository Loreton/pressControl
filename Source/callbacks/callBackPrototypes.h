//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 08.28.51
//

#pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type




    #include "ButtonLongPress_Class.h"

    void pressControlNotificationCB(ButtonLongPress_Class* p);
    void startButtonNotificationCB(ButtonLongPress_Class* p);
    void pumpPressedNotificationCB(ButtonLongPress_Class* p);
    void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Class *p);


