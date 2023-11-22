#include "skiSensor.h"


skiSensor::skiSensor()
{
    ICM20948 IMU;
    AK09916 MAG;
    esp32 controller;

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

    MAG.readStatus2();      //Do I need this?

    //controller.startBLE();
    //controller.startAdvertising();
}



void skiSensor::streamRawValues()
{
    //maybe save this in an instance variable rather than creating two identical variables?
    std::vector<int> Acceleration = getRawAcceleration();
    std::vector<int> RotationalVelocity = getRawRotationalVelocity();
    std::vector<int> Heading = getRawHeading();
    
    std::string type = "motionReading";
        
    std::map<std::string, int> payloadMap;
    payloadMap["rawAccelX"] = Acceleration[0];
    payloadMap["rawAccelY"] = Acceleration[1];
    payloadMap["rawAccelZ"] = Acceleration[2];
    payloadMap["rawGyroX"] = RotationalVelocity[0];
    payloadMap["rawGyroY"] = RotationalVelocity[1];
    payloadMap["rawGyroZ"] = RotationalVelocity[2];
    payloadMap["rawMagX"] = Heading[0];
    payloadMap["rawMagY"] = Heading[1];
    payloadMap["rawMagZ"] = Heading[2];

    message valueMessage(type, payloadMap, "0");
    valueMessage.buildMessage();
    valueMessage.send();

}

void setOutput(char outputtype)
{
    message::s_routerType = outputtype;
}


std::vector<int> skiSensor::getRawAcceleration()
{
    IMU.readRawAcceleration();
    std::vector<int> rawAcceleration = acceleration.getState(); 

    return rawAcceleration;
}


std::vector<int> skiSensor::getRawRotationalVelocity()
{
    IMU.readRawRotationalVelocity();
    std::vector<int> rawRotationalVelocity = rotationalVelocity.getState(); 

    return rawRotationalVelocity;
}


std::vector<int> skiSensor::getRawHeading()
{
    MAG.readRawHeading();
    std::vector<int> rawHeading = heading.getState(); 

    return rawHeading;
}