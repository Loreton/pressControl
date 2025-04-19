/*
// updated by ...: Loreto Notarantonio
// Date .........: 19-04-2025 14.57.44
*/

#ifndef __PUMP_FUNCTIONS_H__
    #define __PUMP_FUNCTIONS_H__
    #include "@a_mainProject.h"


    uint8_t pumpStatusRead(uint8_t fromModule=NO_TELEGRAM);
    uint8_t pumpStateForAlarm(void);
    void    pumpAnomalCondition(void);


#endif