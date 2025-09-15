//
// updated by ...: Loreto Notarantonio
// Date .........: 15-09-2025 15.21.20
//

#pragma once

    // #include <Arduino.h>    // in testa anche per le definizioni dei type
    // #include "main.h"


    #include <ButtonLongPress_Class.h>
    #include <outPinController_Class.h>
    #include <passiveBuzzer_Class.h>

    //* - startButtonHandlers
    // void startButtonHandler(uint8_t pressedLevel);
    void startButtonHandler(ButtonLongPress_Class *p);
    void startButtonNotificationCB(ButtonLongPress_Class* p);

    //* - pumpHndlers
    void pumpHandler(ButtonLongPress_Class *p);
    void pumpNotificationCB(ButtonLongPress_Class* p);


    //* - pressControl
    void pressControlNotificationCB(ButtonLongPress_Class* p);
    void pressControlHandler(ButtonLongPress_Class *p);

    //* - common CallBacks
    void beepNotification(ButtonLongPress_Class *p, uint32_t beep_duration);
    void sendTelegram(const char *message);

    void waitForPulseEnding(outPinController_Class *p, int32_t timeOut=10000);
    void waitForPulseEnding(PassiveBuzzer_Class *p, int32_t timeOut=10000);


    //* - actionStatus
    void chackActionStatus(void);
    void sendStatusToTelegram(bool force=false);

    //* - telegramProcess
    void setTelegramTitle(void);
    void sendNtpSynchedTelegramMessage(void);
    void telegramSendDevicesStatus(void);

    // --- commonUtilitiesFunctions
    void sirenaBitonale(void);