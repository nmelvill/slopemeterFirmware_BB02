#include "BLE.h"

void initializeBLE()
{
    NimBLEDevice::init("BB02");
    
    NimBLEServer *pServer = NimBLEDevice::createServer();
    
    NimBLEService *pIMUService = pServer->createService(IMU_SERVICE_UUID);
    NimBLEService *pDeviceInfo = pServer->createService(DEVICE_INFO_SERVICE_UUID);

}