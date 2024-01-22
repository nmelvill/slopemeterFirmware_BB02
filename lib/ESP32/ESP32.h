#ifndef ESP32_H
#define ESP32_H

#include "Arduino.h"

#define INTERNAL_LED_RED 14
#define INTERNAL_LED_BLUE 15
#define INTERNAL_LED_GREEN 16



class esp32
{
    public:
    esp32();
    void initialize();
    void turnOnInternalRed();
    void turnOnInternalGreen();
    void turnOnInternalBlue();
    void turnOffInternalLED();
    void heartBeatLED();
    static void runHeartBeatLED();


  
    private:
    

};


#endif