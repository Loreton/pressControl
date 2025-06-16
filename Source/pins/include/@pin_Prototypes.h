//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 18.43.52
//

#include <Arduino.h>

// ;  //this line ALSO solves everything!

#ifndef __PIN_PROTOTYPES_H__
    #define __PIN_PROTOTYPES_H__

    void pinsInitialization(void);

    void setup_deBouncedButton(deBouncedButton_t *btn, int pin, const char* name, int pressedLogicLevel);
    // void setup_deBouncedButton2(deBouncedButton_s &btn, int pin, const char* name, int pressedLogicLevel);

    bool read_deBouncedButton(deBouncedButton_t *btn, unsigned long debounceDelay);



#endif
