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

    setOutput();
}

void skiSensor::printRawHeading()
{
    
    //MAG.readStatus1(false);

    MAG.readRawHeading();

    std::vector<int> Heading = heading.getState(); 
    
    Serial.print("X:  ");Serial.print(Heading[0]);
    Serial.print("  Y:  ");Serial.print(Heading[1]);
    Serial.print("  Z:  ");Serial.println(Heading[2]);
}

void skiSensor::setOutput()

{
    bool isSerial = Serial;
    bool isBLE = false;

    if (isSerial==true && isBLE !=true)
        {messageRouter interface("serial");
        }
    
    else if (isSerial==true && isBLE==true)
        {messageRouter interface("both");
        }

    else if (isSerial==false && isBLE==true)
        {messageRouter interface("BLE");
        }
}