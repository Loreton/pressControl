//
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 17.52.09
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@pinOperations.h"


// ---------------------------------
// - project headers files
// ---------------------------------
#include "@pinDefinitions.h"
#include "@readPressedButton.h"




#define fTeleframNotify          true

extern io_input_pin_struct_t *startButton;



// =====================================
// = process startButton
// = viene individuato il livello di presssione
// = inteso come tempo di tenuta di pressione tasto
// = ovviamente intercettiamo il rilascio del tasto per operare
// = nei livelli intermedi inviamo un beep di segnalazione
// =====================================
uint8_t startButtonLongPress() {
    static uint8_t last_level = -1;
    static uint32_t duration = 200;
    static uint16_t frequency;

    uint8_t ret_value = 0;

    // -- va letto prima dell'IF che segue
    uint8_t pressed_level = readLongPressPin(startButton);
    // readLongPressPin(startButton);

    if (startButton->isReleased) {
        // verifichiamo l'ultimo livello raggiunto
        printf1_NFN("%s released on level: %d\n", startButton->pinID, startButton->pressedLevel);
        startButton->pressedLevel=NO_BUTTON_PRESSED;
        startButton->isReleased=false;

        // switch (startButton->pressedLevel) {
        switch (pressed_level) {
            case PRESSED_TIME_01:
                ret_value = PRESSED_TIME_01;
                // pressControlRelayToggle(CALLED_BY_STARTBUTTON);
                break;


            case PRESSED_TIME_02:
                ret_value = PRESSED_TIME_02;
                // loadSuperioreRelayToggle(CALLED_BY_STARTBUTTON);
                break;

            case PRESSED_TIME_03:
                ret_value = PRESSED_TIME_03;
                printf1_NFN("%s released RELEASED_TIME_03: %d\n", startButton->pinID, startButton->pressedLevel);
                break;

        } // end switch
    }
    return ret_value;

} // end function



