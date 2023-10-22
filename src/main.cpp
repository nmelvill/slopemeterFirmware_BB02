#include <Arduino.h>
#include "Wire.h"
//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "skiSensor.h"

void continuousRead();

void setup() {

  skiSensor BB02;

}



void loop() {

  //continuousRead();
  delay(500);
}

void continuousRead()
{
  //Serial.print(IMU.readAccleration()[0]); Serial.print(", ");
  //Serial.print(IMU.readAccleration()[1]); Serial.print(", ");
  //Serial.print(IMU.readAccleration()[2]); Serial.print(", ");
  //Serial.print(IMU.readRotationalVelocity()[0]); Serial.print(", ");
  //Serial.print(IMU.readRotationalVelocity()[1]); Serial.print(", ");
  //Serial.print(IMU.readRotationalVelocity()[2]); Serial.print(", ");
  //Serial.print(MAG.readHeading()[0]); Serial.print(", ");
  //Serial.print(MAG.readHeading()[1]); Serial.print(", ");
  //Serial.print(MAG.readHeading()[2]); Serial.print(", ");
  Serial.println();
}
