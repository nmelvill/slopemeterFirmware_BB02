#include "BLE.h"


BLE::BLE()

{
}


void BLE::initialize()
{
    NimBLEDevice::init("BB02");


    pServer = NimBLEDevice::createServer();
    pTelemetryService = pServer->createService(IMU_SERVICE_UUID);
    pDeviceInfoService = pServer->createService(DEVICE_INFO_SERVICE_UUID);
    
    pAccelCharacteristic = pTelemetryService->createCharacteristic(ACCELERATION_CHARACTERISTIC_UUID,
                                                                    NIMBLE_PROPERTY::READ |
                                                                    NIMBLE_PROPERTY::NOTIFY);

    pRotationCharacteristic = pTelemetryService->createCharacteristic(ROATION_CHARACTERISTIC_UUID,
                                                                    NIMBLE_PROPERTY::READ |
                                                                    NIMBLE_PROPERTY::NOTIFY);

    pHeadingCharacteristic = pTelemetryService->createCharacteristic(HEADING_CHARACTERISTIC_UUID,
                                                                    NIMBLE_PROPERTY::READ |
                                                                    NIMBLE_PROPERTY::NOTIFY);

    pTemperatureCharacteristic = pTelemetryService->createCharacteristic(TEMPERATURE_CHARACTERISTIC_UUID,
                                                                    NIMBLE_PROPERTY::READ |
                                                                    NIMBLE_PROPERTY::NOTIFY);

    pBatteryLifeCharacteristic = pDeviceInfoService->createCharacteristic(BATTERY_LIFE_CHARACTERISTIC_UUID,
                                                                    NIMBLE_PROPERTY::READ |
                                                                    NIMBLE_PROPERTY::NOTIFY);

    startService();
    initializeCharacteristics();
}


void BLE::initializeCharacteristics()
//Initializes characteristics and sets them to 0 for advertising.
{
    pAccelCharacteristic->setValue(0);
    pRotationCharacteristic->setValue(0);
    pHeadingCharacteristic->setValue(0);
    pTemperatureCharacteristic->setValue(0);

    pBatteryLifeCharacteristic->setValue(0);
}


void BLE::startAdvertising()
{
    pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(IMU_SERVICE_UUID); 
    delay(500);

    //Setting scan response to false to preserve battery life
    pAdvertising->setScanResponse(false);
    pAdvertising->start();

}

void BLE::startService()
{
    pTelemetryService->start();
    pDeviceInfoService->start();
}

void BLE::streamAcceleration(uint64_t accelValue)
{
    pAccelCharacteristic->setValue(accelValue);
    pAccelCharacteristic->notify(true);
}

void BLE::streamRotation(uint64_t rotationValue)
{
    pRotationCharacteristic->setValue(rotationValue);
    pRotationCharacteristic->notify(true);
}

void BLE::streamHeading(uint64_t headingValue)
{
    pHeadingCharacteristic->setValue(headingValue);
    pHeadingCharacteristic->notify(true);
}