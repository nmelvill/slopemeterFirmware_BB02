#include "skiSensor.h"

skiSensor::skiSensor()
{
    ICM20948 IMU;
    AK09916 MAG;

    initialize();
}

void skiSensor::initialize()
{
    Serial.begin(115200);
    
    delay(3000);

    IMU.connect();
    IMU.turnOn();
        //In order to use the magnetometer the I2C aux bus of the ICM20948 must be set to pass through mode
    IMU.enablePassThru();
    MAG.turnOn();
    MAG.readStatus2();
}