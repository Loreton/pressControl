//
// updated by ...: Loreto Notarantonio
// Date .........: 30-07-2025 16.15.52
//

#pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type
    #include "main.h"


    #include "ButtonLongPress_Class.h"

    //* - startButtonHandlers
    void startButtonHandler(uint8_t pressedLevel);
    void startButtonNotificationCB(ButtonLongPress_Class* p);

    //* - pumpHndlers
    void pumpHandler(ButtonLongPress_Class *p);
    void pumpNotificationCB(ButtonLongPress_Class* p);


    //* - pressControl
    void pressControlNotificationCB(ButtonLongPress_Class* p);

    //* - common CallBacks
    // void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration);
