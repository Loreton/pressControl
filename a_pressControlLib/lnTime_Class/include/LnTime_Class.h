//
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 17.49.42
//

// LnTime_Class.h
#pragma once

    #include "Arduino.h"
    #include <ESP32Time.h> // ESP32Time.cpp
    #include <map>
    #include <vector> // Use std::vector for flexible arrays

    // #define TIME_BUFFER_LENGTH 16
    #define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3"

    class LnTime_Class {


        private:
            ESP32Time rtc;
            struct tm      m_timeinfo;
            // int8_t m_last_second = -1;
            // int8_t m_last_minute = -1;
            // int8_t m_last_hour = -1;
            // std::map<uint8_t,  uint8_t>  m_at_last_second_map;
            // std::map<uint8_t,  uint8_t>  m_at_last_minute_map;
            // std::map<uint8_t,  uint8_t>  m_at_last_hour_map;



        public:
            // Costruttore
            LnTime_Class();

            // Metodi pubblici
            void setup(uint16_t ntpIntervalTimeSync=2*60); // seconds
            void update(void);
            char *now(void);



            // Metodi per accedere alla struttura tm e all'oggetto rtc (nuovi o modificati per l'accesso esterno)
            struct tm getTimeStruct();
            ESP32Time& getRtcInstance(); // Aggiungi questo metodo per esporre l'istanza rtc
            /**
             * Se timeBuffer è statica e globale.
             * Ogni chiamata a timeStamp sovrascrive il contenuto di timeBuffer,
             * quindi quando stampi più valori nello stesso printf,
             * entrambe le chiamate restituiscono il valore dell'ultima chiamata
             * per tale ragione il buffer deve essere allocato estrnamente
             * ....oppure prestare molta attenzione
            */
            char sharedTimeBUFFER[16];


        #include "LnTime_ntp.h"
        #include "LnTime_timeMethods.h"
        #include "LnTime_seconds.h"
        #include "LnTime_minutes.h"
        #include "LnTime_hours.h"
        #include "LnTime_days.h"

    };

    // Dichiarazioni delle funzioni di stampa esterne
    void printLocalTime(const struct tm *timeinfo_ptr); // Prende un const tm* per chiarezza
    void print_rtc_time(LnTime_Class *p); // Prende un puntatore a LnTime_Class

/**  Spiegazione delle Modifiche in LnTime_Class.h:
    Abbiamo aggiunto un metodo pubblico ESP32Time& getRtcInstance();
    nella classe LnTime_Class. Questo è fondamentale!
    Ti permette di accedere all'istanza privata rtc della classe LnTime_Class dall'esterno,
    in modo che le funzioni di stampa possano chiamare i metodi di ESP32Time (come getDate(), getTime(), ecc.).
    Restituire un riferimento (&) evita di copiare l'oggetto, che è efficiente.

    Le dichiarazioni di printLocalTime e print_rtc_time sono state spostate fuori dalla classe,
    ma all'interno del file .h per renderle visibili ad altri file che includono LnTime_Class.h.
*/




extern LnTime_Class lnTime;