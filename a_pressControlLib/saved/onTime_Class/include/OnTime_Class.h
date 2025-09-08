/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 08.17.07
*/

// #define ON_TIME_CLASS_SECONDS
// #define ON_TIME_CLASS_MINUTES
// #define ON_TIME_CLASS_HOURS



#pragma once
#include <Arduino.h>
#include <ESP32Time.h>
#include <map>
#include <vector>

// Dichiarazione base della classe
class OnTime_Class {
    private:
        ESP32Time rtc;
        struct tm m_timeinfo;

    public:
        OnTime_Class(void);
        void setup();
        void update(void);

    // Include le sezioni modulari
    #include "OnTime_seconds.h"
    #include "OnTime_minutes.h"
    #include "OnTime_hours.h"
    #include "OnTime_days.h"
};

extern OnTime_Class onTime;



extern OnTime_Class onTime;
