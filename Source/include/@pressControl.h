/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 11.18.53
*/

#ifndef __PRESS_CONTROL_FUNCTIONS_H__
    #define __PRESS_CONTROL_FUNCTIONS_H__
    #include "@prjStructures.h"

    void    pressControlRelayToggle(uint8_t caller=NO_TELEGRAM);
    void    pressControlRelayON(uint8_t caller=NO_TELEGRAM, int32_t pulsetime=0);
    void    pressControlRelayOFF(uint8_t caller=NO_TELEGRAM);
    void    pressControlRelaySet(bool state, uint8_t caller=NO_TELEGRAM);
    void    pressControlRelayStatus(uint8_t caller=NO_TELEGRAM);

    void    pressControlProcessTelegramMessage(char *msgText);


#endif