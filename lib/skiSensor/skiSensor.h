#ifndef SKISENSOR_H
#define SKISENSOR_H

#include <Arduino.h>
#include "Wire.h"
//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "ICM20948.h"


class skiSensor
//Main hub and executive of the skiSensor library 
{
    public:
    skiSensor();
    void initialize();
    
    private:
    ICM20948 IMU;
    AK09916 MAG;
};

#endif