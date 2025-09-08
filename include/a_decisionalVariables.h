//
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 18.28.38
//
#pragma once


    // #define ln_ESP32_WROOM_32E_MODULE         1
    // #define ln_ESP32_WROOM_32E_MODULE_2RELAY  2
    // #define ln_ESP32_BOARD_TYPE               ln_ESP32_WROOM_32E_MODULE
    // #define ln_ESP32_BOARD_TYPE               ln_ESP32_WROOM_32E_MODULE_2RELAY

    // #define ln_DEVEL                          1
    // #define ln_PRODUCTION                     2
    // #define ln_RELEASE_TYPE                   ln_DEVEL
    // #define ln_RELEASE_TYPE                   ln_PRODUCTION definito in piorun.sh

    // #define __USE_DUMMY_NOW_TIME__
    // #define __TYPEDEF_STRUCT__


    // definiti in piorun.sh
    //      ln_RELEASE_TYPE
    //      ln_DEVEL
    //      ln_PRODUCTION

    #ifdef __I_AM_MAIN_CPP__
        #if ln_RELEASE_TYPE == ln_PRODUCTION
            #pragma message "siamo in PRODUCTION"
        #else
            #pragma message "siamo in SVIL"
        #endif

        #if ln_ESP32_BOARD_TYPE == ln_ESP32_WROOM_32E_MODULE
            #pragma message "siamo con la board ln_ESP32_WROOM_32E_MODULE"
        #else
            #pragma message "siamo con la board ln_ESP32_WROOM_32E_MODULE_2RELAY"
        #endif

    #endif
