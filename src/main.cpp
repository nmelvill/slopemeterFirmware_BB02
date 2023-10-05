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
  //Serial.println(bytesReceived);
  
  /*This following command works because nbyte is initialized as a fixed width integer of size 16 so 0000000000000000.  Wire.read() only reads a byte
  or 8 bits so 0101010.  <<8 bit shifts 8 bits to the left and the | operator will change each bit if it is a 1.  So it might look like this under the hood
  00000000000000000 = 01110001 <<8 | 10101011 = 10101011 + 01110001.  https://arduino.stackexchange.com/questions/36383/what-does-this-notation-stands-for-wire-read-8-wire-read
  */
  //nbyte = Wire.read() <<8 | Wire.read(); 
  


  Serial.print(IMU.readAccleration()[0]);
  Serial.print(IMU.readAccleration()[1]);
  Serial.print(IMU.readAccleration()[2]);
  Serial.println();

}

// put function definitions here:
