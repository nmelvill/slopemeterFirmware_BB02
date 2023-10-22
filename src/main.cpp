#include <Arduino.h>
#include "Wire.h"
//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "skiSensor.h"

void continuousRead();

skiSensor BB02;

void setup() {
  
BB02.initialize();
}


void loop() {
  //continuousRead();
  delay(500);
  //BB02.printValues();
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
