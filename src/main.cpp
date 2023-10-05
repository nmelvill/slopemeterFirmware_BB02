#include <Arduino.h>
#include "Wire.h"
//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "ICM20948.h"
#include "property.h"

ICM20948 IMU; 

const uint8_t imuAddress = 0x69;

int16_t nbyte;
int8_t WhoAmI;

size_t requestSize = 2; //Needs to be size_t for the requestFrom method to run without error


// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // start serial for output
  Serial.setDebugOutput(true);

                //Initialize IMU Class
  
  IMU.connect();
  IMU.turnOn();
}

void loop() {
  // put your main code here, to run repeatedly:

  Wire.requestFrom(imuAddress, requestSize, true);

  


  Serial.print(IMU.readAccleration()[0]); Serial.print(", ");
  Serial.print(IMU.readAccleration()[1]); Serial.print(", ");
  Serial.print(IMU.readAccleration()[2]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[0]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[1]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[2]); Serial.print(", ");
  Serial.println();

}

// put function definitions here:
