//
// updated by ...: Loreto Notarantonio
// Date .........: 10-06-2025 08.26.13
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@pinOperations.h"
#include "@lnString.h"
#include "@ln_MillisTimer.h"


// ---------------------------------
// - project headers files
// ---------------------------------
#include "@a_mainProject.h"
#include "@pressControl.h"
#include "@pinDefinitions.h"
#include "@ln_telegram.h"
#include "@pump.h"





// extern io_output_pin_struct_t *pressControlRelay;
// extern io_input_pin_struct_t  *pressControlState;





void pressControlToTelegram(uint8_t caller) {
    if (caller == NO_TELEGRAM ) {return; }

    readOutputPin(pressControlRelay);

    if (caller == PUMP_FORCED_OFF   ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>Pump forced off due to long time</b>\n");
    }
    else if (caller == PRESS_CONTROL_ON ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>pressControl ON</b>\n");
    }
    else if (caller == PRESS_CONTROL_OFF ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>pressControl OFF</b>\n");
    }
    else if (caller == ALEXA_REQ ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>Req by Alexa</b>\n");
    }
    else if (caller == PUMP_ON ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>pump on</b>\n");
    }
    else if (caller == PUMP_OFF ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>pump off</b>\n");
    }
    else if (caller == PUMP_STILL_ON ) {
        tgMsg->len = sprintf(tgMsg->data, "<b>pump still ON [lev.%d/%d]</b>\n", pumpState->pressedLevel, pumpState->n_thresholds-2);
    }
    else if (caller != CALLED_BY_TELEGRAM ) {
        tgMsg->len = 0;
    }

    tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "PC relay: %s\n",  str_OffOn[pressControlRelay->is_acted]);
    if (pressControlRelay->pulsetime.cur_duration != 0) {
        uint32_t remaining_pulsetime = remainingPulseTime(pressControlRelay); // calculate remainig pulsTime if active
        tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "remaining: %s\n", pressControlRelay->pulsetime.hms_remaining);
    }


    // dobbiamo leggere lo status dei dispositivi
    delay(500);
    readInputPin(pressControlState);
    readInputPin(pumpState);
    tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "PC status: %s\n", str_OffOn[pressControlState->is_ON]);
    tgMsg->len += sprintf(tgMsg->data + tgMsg->len, "PUMP status: %s\n", str_OffOn[pumpState->is_ON]);

    if (tgMsg->len != 0 && caller != CALLED_BY_TELEGRAM) { // l'invio a telegram viene fatto da ln_telegram.cpp
        sendToTelegram();
    }
     // printf0_NFN("sono qui\n");
}




// ##############################################################
// # Mi è comodo utilizzare le funzioni di ON e OFF
// # perché mi serve che compiano operazioni relative allo stato
// ##############################################################




void pressControlRelayON(uint8_t caller, int32_t mseconds) {
    printf0_NFN("[%s:%d] to ON\n", pressControlRelay->name, pressControlRelay->pin);
    pinPulseON(pressControlRelay, mseconds, false);
    pinOFF(pumpLED);
    pinOFF(pressControlLED);
    pressControlToTelegram(caller);
}

void pressControlRelayOFF(uint8_t caller) {
    printf0_NFN("[%s:%d] to OFF\n", pressControlRelay->name, pressControlRelay->pin);
    pinOFF(pressControlRelay);
    pinOFF(pumpLED);
    pinOFF(pressControlLED);
    pressControlToTelegram(caller);
}



// ##############################################################
// # non uso pinToggle perché mi serve di richiamare le funzioni relayON relaOGG
// ##############################################################
void pressControlRelayToggle(uint8_t caller) {
    if (isActed(pressControlRelay)) {
        pressControlRelayOFF(caller);
    } else {
        pressControlRelayON(caller);
    }
}

void pressControlRelaySet(bool state, uint8_t caller) {
    if (state) {
        pressControlRelayON(caller);
    } else {
        pressControlRelayOFF(caller);
    }
}



void pressControlRelayStatus(uint8_t caller) {
    if (isPulsetimeExpired(pressControlRelay))
        pressControlRelayOFF(TIMER_EXPIRED);
    else
        pressControlToTelegram(caller);
}





extern const PROGMEM char *splittedResult[];

void pressControlProcessTelegramMessage(char *msgText) {
    printf0_NFN("processing telegram message: %s\n", msgText);
    uint8_t words = splitString(msgText, " ");


    char *cmd;
    int32_t mseconds =  0;
    if (words > 0) {
        cmd = (char *)splittedResult[0];
    }
    if (words > 1) {
        mseconds = stringToSeconds((char *)splittedResult[1], ":") * 1000;
    }

    if (strcmp(cmd, "on") == 0) {
        pressControlRelayON(CALLED_BY_TELEGRAM, mseconds);
    }
    else if (strcmp(cmd, "off") == 0) {
        pressControlRelayOFF(CALLED_BY_TELEGRAM);
    }
    else if (strcmp(cmd, "status") == 0) {
        pressControlRelayStatus(CALLED_BY_TELEGRAM);
    }

}