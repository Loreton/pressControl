//
// updated by ...: Loreto Notarantonio
// Date .........: 12-08-2025 16.37.45
//

#pragma once

    // #include <Arduino.h>    // in testa anche per le definizioni dei type
    // #include "main.h"


    #include "ButtonLongPress_Class.h"
    #include "outPinController_Class.h"
    #include "passiveBuzzer_Class.h"

    //* - startButtonHandlers
    void startButtonHandler(uint8_t pressedLevel);
    void startButtonNotificationCB(ButtonLongPress_Class* p);

    //* - pumpHndlers
    void pumpHandler(ButtonLongPress_Class *p);
    void pumpNotificationCB(ButtonLongPress_Class* p);


    //* - pressControl
    void pressControlNotificationCB(ButtonLongPress_Class* p);

    //* - common CallBacks
    void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration);

    void waitForPulseEnding(outPinController_Class *p, int32_t timeOut=10000);
    void waitForPulseEnding(PassiveBuzzer_Class *p, int32_t timeOut=10000);


    //* - sendMessageToTelegram
    void sendMessageToTelegram(String message);
    void sendMessageToTelegram(const char* message);
    void sendMessageToTelegram(const char* message, const char* parseMode);