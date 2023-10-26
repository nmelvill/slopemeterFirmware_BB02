//#include "BLEDevice.h"
//#include "BLEUtils.h"
//#include "BLEServer.h"
#include "skiSensor.h"

skiSensor BB02;

void setup() {
  
BB02.initialize();
}


void loop() {

  delay(150);
  BB02.streamRawValues();
}
