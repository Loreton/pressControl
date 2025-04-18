/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 11.19.37
*/

#ifndef __PUMP_FUNCTIONS_H__
    #define __PUMP_FUNCTIONS_H__
    #include "@prjStructures.h"


    uint8_t pumpStatusRead(uint8_t fromModule=NO_TELEGRAM);
    uint8_t pumpStateForAlarm(void);
    void    pumpAnomalCondition(void);


#endif