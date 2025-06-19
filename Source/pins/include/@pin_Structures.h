/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 11.34.25
*/

#ifndef __PIN_STRUCTURES_H__
    #define __PIN_STRUCTURES_H__

/**
    | ---------------------------------------- | -------------------- | --------------------------
    | Sintassi                                 | Tipo creato          | Uso diretto senza `struct`
    | ---------------------------------------- | -------------------- | --------------------------
    | `typedef struct { int a; } pippo;`       | Alias                | pippo x;
    | `struct pippo { int a; };`               | Struct named         | pippo x;
    | `typedef struct pippo { int a; } pippo;` | Alias + named struct | pippo *x, pippo x
    | ---------------------------------------- | -------------------- | --------------------------
*/

    // ------------------------------------------------------
    // Struttura per mantenere lo stato di ogni pulsante.
    // mi permette di usare la doppia modalità: "named" e "alias"
    // ------------------------------------------------------
    typedef struct deBouncedButton_t {
        int                 pin;               // Il pin GPIO a cui è collegato il pulsante.
        const char*         name;              // Nome del pulsante per identificazione (opzionale, utile per debugging).
        int                 pressedLogicLevel; // Livello logico che indica il pulsante premuto (LOW o HIGH).
        bool                lastButtonState;   // Ultima lettura RAW del pin.
        unsigned long       lastDebounceTime;  // Ultimo momento in cui il pin ha cambiato stato RAW.
        bool                buttonPressed;     // Stato debounced: true se premuto, false se rilasciato.
    } deBouncedButton_t;





    /**
    Esempi di utilizzo
    1....
        #define DEBOUNCED_PINS  3
        deBouncedButton_t  debouncedPINS[DEBOUNCED_PINS+1];
        deBouncedButton_t *startButton1   = &debouncedPINS[0]; // Dichiarazione della struttura per il nostro pulsante.
        deBouncedButton_t *startButton2   = &debouncedPINS[1]; // Dichiarazione della struttura per il nostro pulsante.
        deBouncedButton_t ....
    2....
        deBouncedButton_t pippo;        // crea un instanza
        deBouncedButton_t *startButton = &pippo; // po posso creare un puntatore oppure passarla con l&
    **/


    #ifdef _I_AM_PIN_INITIALIZATION_CPP_
        deBouncedButton_t startButton; // creazione instanza
    #else
        extern deBouncedButton_t startButton; // Dichiarazione della struttura per il nostro pulsante.
    #endif


#endif