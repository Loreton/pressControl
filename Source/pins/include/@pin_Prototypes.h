//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 15.12.09
//

#include <Arduino.h>

// ;  //this line ALSO solves everything!

#ifndef __PIN_PROTOTYPES_H__
    #define __PIN_PROTOTYPES_H__

    // --- pinsInitialization.cpp
    void pinsInitialization(void);

    // --- debouncedButton.cpp deBouncedButton_t *pippo (userà ->)
    void setup_deBouncedButton(deBouncedButton_t *btn, int pin, const char* name, int pressedLogicLevel);

    // --- debouncedButton.cpp deBouncedButton_t pippo (userà .)
    void setup_deBouncedButton(deBouncedButton_t &btn, int pin, const char* name, int pressedLogicLevel);

    bool read_deBouncedButton(deBouncedButton_t *btn, unsigned long debounceDelay=300);


    // --- actionProcess.cpp
    void startButton_action(void);


#endif
