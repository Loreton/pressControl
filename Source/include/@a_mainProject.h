//
// updated by ...: Loreto Notarantonio
// Date .........: 29-06-2025 17.02.47
//


#pragma once
    // doesn't work
    // #include "@debouncedButton_sClass.h"
    // #include "@logMacros.h"


    #include "@a_decisionalVariables.h"
    #include "@pin_Definitions.h"
    #include "@pin_Prototypes.h"



    #ifdef __I_AM_MAIN_CPP__
        // L'inizializzazione avviene tramite il costruttore.
        DebouncedButton_Class startButton; // Dichiarazione della struttura function inside.
    #else
        // extern DebouncedButton_Class startButton; // Dichiarazione della struttura function inside.
    #endif


