/*
// updated by ...: Loreto Notarantonio
// Date .........: 22-08-2025 18.07.06
*/

#pragma once
    #include <Arduino.h> // ESP32Time.cpp
    #include <ESP32Time.h> // ESP32Time.cpp


    // Classe per gestire il rilevamento della pressione lunga di un pulsante.
    class OnTime_Class {
        public: // Membri pubblici
            OnTime_Class(void);

            bool atSecond(uint8_t second);
            bool atMinute(uint8_t minute);
            bool onSecondModulo(uint8_t seconds);
            bool onMinuteModulo(uint8_t minutes);

        private:
            ESP32Time       rtc;
            struct tm       m_timeinfo;
            uint8_t         m_last_second;
            uint8_t         m_curr_second;

            uint8_t         m_last_minute;
            uint8_t         m_curr_minute;
    };