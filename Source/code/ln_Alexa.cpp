//
// updated by ...: Loreto Notarantonio
// Date .........: 28-05-2025 08.08.01
// ref: https://randomnerdtutorials.com/alexa-echo-with-esp32-and-esp8266/
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <fauxmoESP.h>
fauxmoESP fauxmo;

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@mainStructures.h"


// ---------------------------------
// - project headers files
// ---------------------------------s

#define RELE_01 "rele_uno"
#define RELE_02 "rele_due"


extern io_output_pin_struct_t *pressControlRelay;


// ###############################################################################
// il device non posso passarlo come parametro altrinmenti incappo nell'errore: error: 'device01' is not captured
// perhcé lo considera local var e siccome fauxmo utilizza lambda la cosa dà problemi.
// ref: https://forum.arduino.cc/t/help-needed-with-error-myvariable-is-not-captured/1187002/2
// ###############################################################################
void AlexaSetup() {
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    fauxmo.enable(true);
    // Add virtual devices
    uint8_t device_01 = fauxmo.addDevice(pressControlRelay->name);
    printf0_NFN("Alexa set up %s, deviceID: %d\n", pressControlRelay->name, device_01);

    fauxmo.onSetState([]
        (unsigned char device_id, const char * device_name, bool state, unsigned char value) {
            printf0_NFN("Alexa Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

            if ( (strcmp(device_name, pressControlRelay->name) == 0) ) {
                pressControlRelay->alexa_request = true;
                pressControlRelay->alexa_status = state;
            }
        }
    );

}


void chackAlexa() {
    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();
}
