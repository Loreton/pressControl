//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 19.46.27
//

#pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type



#ifdef __BUTTONLONGPRESS_CLASS__

    #include "ButtonLongPress_Class.h"

    void pressControlNotificationCB(ButtonLongPress_Class* p);
    void startButtonNotificationCB(ButtonLongPress_Class* p);
    void pumpPressedNotificationCB(ButtonLongPress_Class* p);
    void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Class *p);


#else

    #include "ButtonLongPress_Struct.h"

    void pressControlNotificationCB(ButtonLongPress_Struct* p);
    void startButtonNotificationCB(ButtonLongPress_Struct* p);
    void pumpPressedNotificationCB(ButtonLongPress_Struct* p);
    void beepNotification(ButtonLongPress_Struct *p, uint32_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Struct *p);
#endif