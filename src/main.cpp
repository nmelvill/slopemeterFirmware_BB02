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
  Serial.print(IMU.readAccleration()[0]); Serial.print(", ");
  Serial.print(IMU.readAccleration()[1]); Serial.print(", ");
  Serial.print(IMU.readAccleration()[2]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[0]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[1]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[2]); Serial.print(", ");
  /*Serial.print(IMU.readHeading()[0]); Serial.print(", ");
  Serial.print(IMU.readHeading()[1]); Serial.print(", ");
  Serial.print(IMU.readHeading()[2]); Serial.print(", ");*/
  Serial.println();

  delay(100);
}


