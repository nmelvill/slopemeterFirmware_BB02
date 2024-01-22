#include "ESP32.h"
#include "common.h"


esp32::esp32()
{
}

void esp32::initialize() 
{
    //pinMode (INTERNAL_LED_RED, OUTPUT);

    ESP_LOGD(TAG, "ESP32 Initialized");

}

void esp32::turnOnInternalRed()
{
    digitalWrite(INTERNAL_LED_RED, LOW); // Red ON
    digitalWrite(INTERNAL_LED_GREEN, HIGH);  // Green OFF
    digitalWrite(INTERNAL_LED_BLUE, HIGH); // Blue OFF
 }


void esp32::turnOnInternalGreen()
{
    digitalWrite(INTERNAL_LED_RED, HIGH); // Red OFF
    digitalWrite(INTERNAL_LED_GREEN, LOW);  // Green ON
    digitalWrite(INTERNAL_LED_BLUE, HIGH); // Blue OFF
}


void esp32::turnOnInternalBlue()
{
    digitalWrite(INTERNAL_LED_RED, HIGH); // Red OFF
    digitalWrite(INTERNAL_LED_GREEN, HIGH);  // Green OFF
    digitalWrite(INTERNAL_LED_BLUE, LOW); // Blue ON
}

void esp32::turnOffInternalLED()
{
    digitalWrite(INTERNAL_LED_RED, HIGH); // Red OFF
    digitalWrite(INTERNAL_LED_GREEN, HIGH);  // Green OFF
    digitalWrite(INTERNAL_LED_BLUE, HIGH); // Blue OFF
}


void esp32::heartBeatLED()
{
    
    turnOnInternalRed();
    delay(100);
    turnOffInternalLED();
    delay(100);
    turnOnInternalRed();
    delay(100);
    turnOffInternalLED();
    delay(1500);


}

 /*void esp32::runHeartBeatLED()
 {
     xTaskCreate(heartBeatLED, "HeartBeat", 2048, NULL, 3, NULL);
 }*/