#include <Arduino.h>
#include "Wire.h"
//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "ICM20948.h"
#include "property.h"

ICM20948 IMU;
AK09916 MAG;

//const uint8_t imuAddress = 0x69;

//int16_t nbyte;
int8_t WhoAmI;

void continuousRead();

void setup() {

  Serial.begin(115200);  // start serial for output
  Serial.setDebugOutput(true);

  delay(3000);

  IMU.connect();
  IMU.turnOn();
  //In order to use the magnetometer the I2C aux bus of the ICM20948 must be set to pass through mode
  IMU.enablePassThru();
  MAG.turnOn();

}

void loop() {

  continuousRead();
  delay(500);
}

void continuousRead()
{
  Serial.print(IMU.readAccleration()[0]); Serial.print(", ");
  Serial.print(IMU.readAccleration()[1]); Serial.print(", ");
  Serial.print(IMU.readAccleration()[2]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[0]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[1]); Serial.print(", ");
  Serial.print(IMU.readRotationalVelocity()[2]); Serial.print(", ");
  Serial.print(MAG.readHeading()[0]); Serial.print(", ");
  Serial.print(MAG.readHeading()[1]); Serial.print(", ");
  Serial.print(MAG.readHeading()[2]); Serial.print(", ");
  Serial.println();
}
