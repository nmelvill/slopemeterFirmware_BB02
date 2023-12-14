
#include "skiSensor.h"


skiSensor BB02;

void setup() 
{

  BB02.initialize();

}


void loop() {
  delay(3000);
  BB02.streamRawValuesToBLE();
  //BB02.streamRawValuesToSerial();
}

