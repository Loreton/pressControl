/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 18.43.19
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@a_mainProject.h"
// #include "@pin_Structures.h"


/**
 * @brief Inizializza un pulsante configurando il pin e lo stato iniziale.
 *
 * @param btn Riferimento alla struttura ButtonState del pulsante.
 * @param pin Il numero del pin GPIO a cui è collegato il pulsante.
 * @param name Una stringa che identifica il pulsante (es. "MyButton").
 * @param pressedLogicLevel Il livello logico (LOW o HIGH) che indica che il pulsante è premuto.
 * - Usa LOW per un pulsante collegato tra pin e GND (con INPUT_PULLUP).
 * - Usa HIGH per un pulsante collegato tra pin e VCC (con INPUT, richiede pull-down esterno).
 */
void setup_deBouncedButton(deBouncedButton_t *btn, int pin, const char* name, int pressedLogicLevel) {
    btn->pin = pin;
    btn->name = name;
    btn->pressedLogicLevel = pressedLogicLevel;

    // Configura il pin in base al pressedLogicLevel.
    if (pressedLogicLevel == LOW) {
        pinMode(btn->pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.
    } else {
        pinMode(btn->pin, INPUT);        // Se premuto porta a HIGH, non usa pull-up interno (richiede pull-down esterno se flottante).
    }
    // pinMode(btn->pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.

    btn->lastButtonState = digitalRead(btn->pin); // Legge lo stato iniziale.
    btn->lastDebounceTime = 0;
    btn->buttonPressed = false; // Stato iniziale debounced.
}


// ------------------------------------
// deBouncedButton_s startButton;
// deBouncedButton_s startButton2;
// ------------------------------------
#ifdef __XXXXXXXXXX
void setup_deBouncedButton2(deBouncedButton_s &btn, int pin, const char* name, int pressedLogicLevel) {
    btn.pin = pin;
    btn.name = name;
    btn.pressedLogicLevel = pressedLogicLevel;

    // Configura il pin in base al pressedLogicLevel.
    if (pressedLogicLevel == LOW) {
        pinMode(btn.pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.
    } else {
        pinMode(btn.pin, INPUT);        // Se premuto porta a HIGH, non usa pull-up interno (richiede pull-down esterno se flottante).
    }
    // pinMode(btn.pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.

    btn.lastButtonState = digitalRead(btn.pin); // Legge lo stato iniziale.
    btn.lastDebounceTime = 0;
    btn.buttonPressed = false; // Stato iniziale debounced.
}

#endif






/**
 * @brief Legge lo stato di un pulsante con debounce.
 * Questa funzione deve essere chiamata ripetutamente nel loop principale.
 *
 * @param btn Riferimento alla struttura ButtonState del pulsante da leggere.
 * @return true se il pulsante è stato RILASCIATO dopo una pressione valida; false altrimenti.
 */
bool read_deBouncedButton(deBouncedButton_t *btn, unsigned long debounceDelay) {

    // Legge lo stato attuale del pin (lettura RAW).
    bool reading = digitalRead(btn->pin);

    // Se la lettura RAW è cambiata rispetto all'ultima lettura RAW, resetta il timer di debounce.
    if (reading != btn->lastButtonState) {
        btn->lastDebounceTime = millis();
    }

    // Se è trascorso il tempo di debounce e lo stato RAW è stabile.
    if ((millis() - btn->lastDebounceTime) > debounceDelay) {
        // Se lo stato RAW stabile è diverso dallo stato debounced corrente, aggiorna lo stato debounced.
        if (reading != btn->buttonPressed) {
            btn->buttonPressed = reading; // Aggiorna lo stato debounced del pulsante.

            // Se il pulsante è stato RILASCIATO.
            // Il pulsante è rilasciato quando il suo stato debounced è DIVERSO da `pressedLogicLevel`.
            if (btn->buttonPressed != btn->pressedLogicLevel) {
                return true; // Il pulsante è stato rilasciato dopo una pressione valida.
            }
        }
    }

    // Salva la lettura RAW attuale per la prossima iterazione.
    btn->lastButtonState = reading;
    return false; // Il pulsante non è stato rilasciato in questo ciclo.
}


