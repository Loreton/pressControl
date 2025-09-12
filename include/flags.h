//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 14.27.10
//

//--- flags.h

#pragma once
    typedef struct {
        unsigned ON                       : 1;
        unsigned OFF                      : 1;
        unsigned firstRun                 : 1;
        unsigned PressControlTimeExausted : 1; // indica che il pressContro ha terminato il tempo massiomo di ON
        unsigned PUMP_ALARM               : 1;
        unsigned ascendent                : 1;
        unsigned discendent               : 1;
        unsigned modulo_10_seconds        : 1;
        unsigned modulo_30_seconds        : 1;
        unsigned modulo_02_minutes        : 1;
        unsigned modulo_03_minutes        : 1;
        unsigned modulo_05_minutes        : 1;
        unsigned modulo_30_minutes        : 1;
        unsigned fonDay                   : 1;
        unsigned fonHour                  : 1;
        unsigned fonMinute                : 1;
        unsigned fonSecond                : 1;
    } ProjectFlags;    // ProjectFlags flags = {0}; // Tutti i flag inizialmente a 0 (false)



    #ifdef __I_AM_MAIN_CPP__
        ProjectFlags f = {0}; // Tutti i flag inizialmente a 0 (false)
        f.ON                       = 1;
        f.OFF                      = 0;
        f.firstRun                 = 1;
        f.PressControlTimeExausted = 0; // indica che il pressContro ha terminato il tempo massiomo di ON
        f.PUMP_ALARM               = 0;
        f.ascendent                = 1;
        f.discendent               = 0;
        f.modulo_10_seconds        = 0;
        f.modulo_30_seconds        = 0;
        f.modulo_02_minutes        = 0;
        f.modulo_03_minutes        = 0;
        f.modulo_05_minutes        = 0;
        f.modulo_30_minutes        = 0;
        f.fonDay                   = 0;
        f.fonHour                  = 0;
        f.fonMinute                = 0;
        f.fonSecond                = 0;
    #else
        extern ProjectFlags f;
    #endif


    /*
        void setFlags(ProjectFlags* pf, unsigned f1, unsigned f2, unsigned f3) {
            pf->flag01 = f1;
            pf->flag02 = f2;
            pf->flag03 = f3;
        }

        setFlags(&flags, 1, 0, 1); // Attiva flag01 e flag03
    */

