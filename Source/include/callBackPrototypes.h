//
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 13.58.47
//

#pragma once

#include <Arduino.h>    // in testa anche per le definizioni dei type

    #include "ButtonLongPress_Struct.h"
    // #include "PinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

    // struct ButtonLongPress_Struct;
    // typedef void (*ButtonCallback)(struct ButtonLongPress_Struct* self);
    // typedef void (*BeepCallBack)(struct ButtonLongPress_Struct* self, uint16_t beep_duration);

    // void myButtonHandler(ButtonLongPress_Struct* btn);

    // potential callback prototypes
    // void startButtonNotificationHandlerCB(ButtonLongPress_Struct* p);
    // void pumpStateNotificationHandlerCB(ButtonLongPress_Struct* p);
    void beepNotification(ButtonLongPress_Struct *p, uint16_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Struct *p);
    void startButtonHandler(ButtonLongPress_Struct *p);
    // void showStatusCB(ButtonLongPress_Struct *p);

    void beepNotification(ButtonLongPress_Struct *p, uint16_t beep_duration);
