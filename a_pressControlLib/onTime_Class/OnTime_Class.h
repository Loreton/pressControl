/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-09-2025 09.09.21
*/

#pragma once
    #include <Arduino.h> // ESP32Time.cpp
    #include <ESP32Time.h> // ESP32Time.cpp
    #include <map>
    #include <vector> // Use std::vector for flexible arrays



    // Classe per gestire il rilevamento della pressione lunga di un pulsante.
    class OnTime_Class {
        private:
            ESP32Time rtc;
            struct tm      m_timeinfo;

        public:
            // Costruttore
            OnTime_Class(void);
            void setup();
            void update(void);






        // -------------------------------
        // - OnTime_Secondi
        // -------------------------------
        private:
            int8_t m_last_second = -1;
            int8_t m_at_last_second = -1;
            int8_t m_at_last_vector_second = -1;

            // --- per atSecond(uint8_t sec)
            std::map<uint32_t, uint32_t> m_last_epoch_seconds_map;  // LnTime_Class_StdMap.txt
            std::map<uint8_t, int8_t> m_at_last_second_map;

            // --- per i vector
            std::vector<uint8_t> m_at_seconds_to_monitor; // array dei secondi da controllare
            std::map<uint8_t, bool> m_at_second_flags;    // map contenente lo stato dei secondi


        public:
            bool onSecond();                     // on second change
            bool atSecond(uint8_t second);
            bool onSecondModulo(uint32_t modulo, bool trueOnFirstRun=false); // on second xx modulo  (ex.: atSecondModulo(20) return true at second 20, 40, 0)
            bool atSecondVectorDedicated(uint8_t second);

            // --- dedicata alla versione con i vector...
            bool getAtSecondFlag(uint8_t second);
            void setAtSecondFlag(const std::vector<uint8_t>& seconds);
            void updateAtSecondFlags(void);



        // -------------------------------
        // - OnTime_Minuti .....
        // -------------------------------
        private:
            std::map<uint32_t, uint32_t> m_last_epoch_minutes_map;
            std::map<uint8_t, int8_t> m_at_last_minute_map;
            std::map<uint8_t, bool> m_at_minute_flags;  // bool getMinuteFlag(uint8_t minute);
            std::vector<uint8_t> m_at_minutes_to_monitor;
            int8_t m_last_minute = -1;
            int8_t m_at_last_minute = -1;


        public:
            bool onMinute();                     // on minute change
            bool atMinute(uint8_t minute);
            bool atMinute2(uint8_t minute);
            // bool onMinuteModulo(uint32_t modulo, bool trueOnFirstRun=false); // on minute xx modulo  (ex.: atMinuteModulo(20) return true at minute 20, 40, 80, 0)
            // bool getAtMinuteFlag(uint8_t minute);
            // void setAtMinuteFlag(const std::vector<uint8_t>& minutes);
            // void updateAtMinuteFlags(void);


        // -------------------------------
        // - OnTime_Hours .....
        // -------------------------------
        private:
            int8_t m_last_hour = 99;
            // std::map<uint32_t, uint32_t> m_last_epoch_hours_map;
            std::map<uint8_t, int8_t> m_at_last_hour_map;
            // std::map<uint8_t, bool> m_at_hour_flags;
            // std::vector<uint8_t> m_at_hours_to_monitor;

        public:
            bool onHour();                     // on hour change


        // -------------------------------
        // - OnTime_Days .....
        // -------------------------------
        private:
            int8_t m_last_yday = 99;
            // std::map<uint32_t, uint32_t> m_last_epoch_ydays_map;
            // std::map<uint8_t, int8_t> m_at_last_yday_map;
            // std::map<uint8_t, bool> m_at_yday_flags;
            // std::vector<uint8_t> m_at_ydays_to_monitor;

        public:
            bool onDay();                     // on hour change


    };

extern OnTime_Class onTime;
