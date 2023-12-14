#include "skiSensor.h"



skiSensor::skiSensor()
{
    ICM20948 IMU;
    AK09916 MAG;
    esp32 controller;
    BLE BLEProcess;

}


void skiSensor::initialize()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    
    delay(3000);

    IMU.connect();
    IMU.turnOn();
    
    //In order to use the magnetometer the I2C aux bus of the ICM20948 must be set to pass through mode
    IMU.enablePassThru();
    MAG.turnOn();

    MAG.readStatus2();      //Do I need this?
    
    BLEProcess.initialize();
    delay(500);

    BLEProcess.startAdvertising();
}



void skiSensor::streamRawValuesToSerial()
{
    //maybe save this in an instance variable rather than creating two identical variables?
    std::vector<int16_t> Acceleration = getRawAcceleration();
    std::vector<int16_t> RotationalVelocity = getRawRotationalVelocity();
    std::vector<int16_t> Heading = getRawHeading();
    

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


std::vector<int16_t> skiSensor::getRawAcceleration()
{
    IMU.readRawAcceleration();
    std::vector<int16_t> rawAcceleration = acceleration.getState(); 

    return rawAcceleration;
}


std::vector<int16_t> skiSensor::getRawRotationalVelocity()
{
    IMU.readRawRotationalVelocity();
    std::vector<int16_t> rawRotationalVelocity = rotationalVelocity.getState(); 

    return rawRotationalVelocity;
}


std::vector<int16_t> skiSensor::getRawHeading()
{
    MAG.readRawHeading();
    std::vector<int16_t> rawHeading = heading.getState(); 

    return rawHeading;
}

void skiSensor::streamRawValuesToBLE()
{

    //Combine separate values into one 64bit integer to send over BLE
    uint64_t Acceration3Axis = serializeVector(getRawAcceleration());
    uint64_t Rotation3Axis = 0;
    uint64_t Heading3Axis = 0;

    BLEProcess.streamAcceleration(Acceration3Axis);
    //BLEProcess.stramRotation(Rotation3Axis);
    //BLEProcess.streamHeading(Heading3Axis);
}


uint64_t skiSensor::serializeVector(std::vector<int16_t> inputVector)
//Converts a vector of three 16bit values to a 64bit unsigned integer in Big Endian Format using signed magnitude
/* 
Output Example: 0x0000FA70CE6828D0
N/A   x      y      z
0000  FA70   CE68   28D0
0000 -1424  -12696  10448

*/
{
    #define FIRST_BIT_MASK 0x0000FFFF00000000
    #define SECOND_BIT_MASK 0X00000000FFFF0000
    #define THIRD_BIT_MASK 0x000000000000FFFF

    uint64_t outputInteger = 0;

    outputInteger |=  ((uint64_t) inputVector[0] << 32) & FIRST_BIT_MASK |  
                        ((uint64_t) inputVector[1] << 16) & SECOND_BIT_MASK |  
                        (uint64_t) inputVector[2] & THIRD_BIT_MASK;

    return outputInteger;

}