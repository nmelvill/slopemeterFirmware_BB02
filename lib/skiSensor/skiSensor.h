#ifndef SKISENSOR_H
#define SKISENSOR_H


#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
static const char* TAG = "BB02";

#include <Arduino.h>
#include "Wire.h"
#include "ble.h"

#include "ICM20948.h"
#include "state.h"
#include <array>
#include "ESP32.h"


#include "esp_log.h"




class skiSensor
//Main hub and executive of the skiSensor library 
{
    public:
      
    skiSensor(BLEInterface* pBLEInterface);
    
    void initialize();
    void run();
    void streamRawValuesToSerial();
    void streamRawValuesToBLE();
    void setOutput(char outputType = 0){message::s_routerType = outputType;}  //0 = Serial, 1= BLE, 2= Both

    std::vector<int16_t> getRawAcceleration();
    std::vector<int16_t> getRawRotationalVelocity();
    std::vector<int16_t> getRawHeading();
    

    enum devicestates {
    //This is not a user-modifiable value
        starting,
        ready,
        running,
        error
    };


    private:
    
    ICM20948 IMU;
    AK09916 MAG;
    esp32 controller;
    BLEInterface *pBLEProcess;

    MotionState acceleration{true, 6};
    MotionState rotationalVelocity{true, 6};
    MotionState heading{false, 6};

    uint64_t serializeVector(std::vector<int16_t> inputVector);

    int devicemode;
    int devicestate;
    //methods

};

#endif