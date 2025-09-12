//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 17.11.06
//

//--- flags.h

#pragma once
    typedef struct {
        unsigned ON                         : 1;
        unsigned OFF                        : 1;
        unsigned firstRun                   : 1;
        unsigned PressControlTimeExausted   : 1; // indica che il pressContro ha terminato il tempo massiomo di ON
        unsigned PUMP_ALARM                 : 1;
        unsigned ascendent                  : 1; // per la scala del passive buzzer
        unsigned discendent                 : 1; // per la scala del passive buzzer
        unsigned modulo_10_seconds          : 1; // indica il raggiungimento del tempo indicato
        unsigned modulo_30_seconds          : 1; // indica il raggiungimento del tempo indicato
        unsigned modulo_02_minutes          : 1; // indica il raggiungimento del tempo indicato
        unsigned modulo_03_minutes          : 1; // indica il raggiungimento del tempo indicato
        unsigned modulo_05_minutes          : 1; // indica il raggiungimento del tempo indicato
        unsigned modulo_30_minutes          : 1; // indica il raggiungimento del tempo indicato
        unsigned onDay                      : 1; // indica il raggiungimento del tempo indicato
        unsigned onHour                     : 1; // indica il raggiungimento del tempo indicato
        unsigned onMinute                   : 1; // indica il raggiungimento del tempo indicato
        unsigned onSecond                   : 1; // indica il raggiungimento del tempo indicato
        unsigned force                      : 1;
        unsigned noForce                    : 1;
        unsigned firstPumpAlarmTime         : 1; // per gestire la segnalazione del pumpAlrm
        unsigned firstPressControlAlarmTime : 1; // per gestire la segnalazione del pressControl Alarm
        unsigned waitForPulseEnding         : 1; // per indicare al buzzer o led di attendere la fine dell'impulso anche se async...
        unsigned withMilliSec               : 1; // lnTime.msecToHMS() indica che includiamo anche i msec nell'output
    } ProjectFlags;    // ProjectFlags flags = {0}; // Tutti i flag inizialmente a 0 (false)



    #ifdef __I_AM_MAIN_CPP__
        ProjectFlags f = {0}; // Tutti i flag inizialmente a 0 (false)
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

    void initFlags(void);