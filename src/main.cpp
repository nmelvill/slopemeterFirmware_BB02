#include "skiSensor.h"
#include "ble.h"

BLEInterface* BLEProcess = new BLEInterface;
skiSensor* BB02 = new skiSensor(BLEProcess);

void setup() 
{

  BB02->initialize();
  BLEProcess->initialize();
  BLEProcess->startAdvertising();

}


void loop() {
  delay(3000);
  BB02->run();
}

