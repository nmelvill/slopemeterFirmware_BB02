#include "skiSensor.h"
#include "ble.h"

BLEInterface* BLEProcess = new BLEInterface;
skiSensor* BB02 = new skiSensor(BLEProcess);
esp32* pEsp32 = new esp32;

void startHeartBeat(void *pvParameters);
BaseType_t xReturned;

void setup() 
{

  
  BB02->initialize();
  BLEProcess->initialize();
  BLEProcess->startAdvertising();

  pEsp32->initialize();

  //Starting a thread to run a non-blocking heartbeat pattern on the LED
  xReturned = xTaskCreate(startHeartBeat, 
                          "HeartBeat", 
                          1024, 
                          NULL, 
                          tskIDLE_PRIORITY, 
                          NULL);


}


void loop() {
  
  delay(500);
  BB02->run();


}

void startHeartBeat(void *pvParameters)
{
  for( ;; )
    {
      pEsp32->heartBeatLED();
    }

}