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


class skiSensor
//Main hub and executive of the skiSensor library 
{
    public:
    
    skiSensor();
    
    void initialize();
    
    void printRawHeading();
    


    private:
    
    ICM20948 IMU;
    AK09916 MAG;

    MotionState acceleration{true, 6};
    MotionState rotationalVelocity{true, 6};
    MotionState heading{false, 6};

    void setOutput();

    //methods

};

#endif