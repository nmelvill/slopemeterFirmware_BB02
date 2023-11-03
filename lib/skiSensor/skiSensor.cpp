#include "skiSensor.h"

skiSensor::skiSensor()
{
    ICM20948 IMU;
    AK09916 MAG;

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



void skiSensor::streamRawValues()
{
    IMU.readRawAcceleration();
    std::vector<int> Acceleration = acceleration.getState(); 
    
    IMU.readRawRotationalVelocity();
    std::vector<int> RotationalVelocity = rotationalVelocity.getState(); 
    
    MAG.readRawHeading();
    std::vector<int> Heading = heading.getState(); 
    
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