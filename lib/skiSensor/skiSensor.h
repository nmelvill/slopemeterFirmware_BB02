#ifndef SKISENSOR_H
#define SKISENSOR_H

#include <Arduino.h>
#include "Wire.h"
//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "ICM20948.h"
#include "state.h"
#include <array>
#include "ESP32.h"




class skiSensor
//Main hub and executive of the skiSensor library 
{
    public:
    
    skiSensor();
    
    void initialize();
    void streamRawValues();
    void setOutput(char outputType = 0){message::s_routerType = outputType;}  //0 = Serial, 1= BLE, 2= Both

    std::vector<int> getRawAcceleration();
    std::vector<int> getRawRotationalVelocity();
    std::vector<int> getRawHeading();

    

    private:
    
    ICM20948 IMU;
    AK09916 MAG;
    esp32 controller;

    MotionState acceleration{true, 6};
    MotionState rotationalVelocity{true, 6};
    MotionState heading{false, 6};

    //methods

};

#endif