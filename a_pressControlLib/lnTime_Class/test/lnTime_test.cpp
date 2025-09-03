//
// updated by ...: Loreto Notarantonio
// Date .........: 03-09-2025 15.37.28
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include "lnLogger_Class.h"

#include "LnTime_Class.h"


// LnTime_Class lnTime; // Crea un'istanza della classe LnTime

void setup() {
    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    // lnTime.setup(2*60); // Chiama il metodo setup della tua istanza di LnTime
    const std::vector<uint8_t> seconds = {0, 1, 15, 30, 45, 120};
    const std::vector<uint8_t> minutes = {0, 1, 3, 5, 45, 120};
    lnTime.setup(2 * 60);
    lnTime.setAtMinuteFlag(minutes);
    lnTime.setAtSecondFlag(seconds);
    Serial.println("Sono lnTime TEST");
}



bool    first_run=true;


// void Seconds(void) {
//     static const int8_t array[] = {0, 1, 15, 30, 45, 120};
//     for (int i = 0; i < sizeof(array); i++) {
//         int8_t value = array[i];

//         if (lnTime.atSecond(value)) {
//             LOG_WARN("Second at %d", value);
//         }

//         if (lnTime.onSecondModulo(value)) {
//             LOG_SPEC("Second on %d modulo!", value);
//         }
//     }
// }


// void Minutes(void) {
//     static const int8_t array[] = {0, 1, 3, 5, 45, 120};
//     for (int i = 0; i < sizeof(array); i++) {
//         int8_t value = array[i];
//         if (lnTime.atMinute(value)) {
//             LOG_WARN("Minute at %d", value);
//         }

//         if (lnTime.onMinuteModulo(value)) {
//             LOG_SPEC("Minute on %d modulo!", value);
//         }
//     }
// }


// #############################################################
// #
// #############################################################
int8_t counter=0;
void loop() {
    uint16_t value;
    uint16_t minute;
    uint16_t second;
    lnTime.update();
    // lnTime.updateAtMinuteFlags();
    // lnTime.updateAtSecondFlags();


    if (lnTime.onSecond()) {
        // {;i("Current time: %s", lnTime.nowTime());
        // Stampa il tempo completo usando la funzione dedicata
        // struct tm current_time_info = lnTime.getTimeStruct();
        // printLocalTime(&current_time_info); // Chiamata alla funzione esterna
        LOG_INFO("It's a new second!");
        // print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }
    if (lnTime.onMinute()) {
        LOG_INFO("It's a new minute!");
    }

    if (lnTime.onHour()) {
        LOG_INFO("It's a new hour!");
    }

    // --- Usa i flag per altre logiche ---
    // Ad esempio, controlla un flag specifico
    if (lnTime.getAtSecondFlag(0)) {
        LOG_WARN("Il flag del 0° secondo è attivo!");
    }

    if (lnTime.getAtSecondFlag(1)) {
        LOG_WARN("Il flag del 1° secondo è attivo!");
    }

    if (lnTime.getAtSecondFlag(15)) {
        LOG_WARN("Il flag del 15° secondo è attivo!");
    }

    if (lnTime.getAtSecondFlag(30)) {
        LOG_WARN("Il flag del 30° secondo è attivo!");
    }

    if (lnTime.getAtSecondFlag(45)) {
        LOG_WARN("Il flag del 45° secondo è attivo!");
    }

    if (lnTime.getAtMinuteFlag(1)) {
        LOG_WARN("Il flag del 1° minuto è attivo!");
    }

    if (lnTime.getAtMinuteFlag(3)) {
        LOG_WARN("Il flag del 3° minuto è attivo!");
    }



    if (lnTime.getAtMinuteFlag(5)) {
        LOG_WARN("Il flag del 5° minuto è attivo!");
    }



    // Seconds();
    // Minutes();
    // conArray();


    if (first_run) {
        first_run=false;
        LOG_INFO("fine primo loop....\n\n");
    }
    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


