#include "BLE.h"


/** Handler class for characteristic actions */
class CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
    
    
    void onRead(NimBLECharacteristic* pCharacteristic){
        //Serial.print(pCharacteristic->getUUID().toString().c_str());
        //Serial.print(": onRead(), value: ");
        //Serial.println(pCharacteristic->getValue().c_str());
    };


    void onWrite(NimBLECharacteristic* pCharacteristic) {
        //Serial.print(pCharacteristic->getUUID().toString().c_str());
        //Serial.print(": onWrite(), value: ");
        //Serial.println(pCharacteristic->getValue().c_str());
    };
    /** Called before notification or indication is sent,
     *  the value can be changed here before sending if desired.
     */


    void onNotify(NimBLECharacteristic* pCharacteristic) {
        //Serial.println("Sending notification to clients");
    };


};


static CharacteristicCallbacks characteristicCallbacks;


BLEInterface::BLEInterface() = default;


void BLEInterface::initialize()
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
                                                                    NIMBLE_PROPERTY::READ);

    pBatteryLifeCharacteristic = pDeviceInfoService->createCharacteristic(BATTERY_LIFE_CHARACTERISTIC_UUID,
                                                                    NIMBLE_PROPERTY::READ);

    pDeviceModeCharacteristic = pDeviceInfoService->createCharacteristic(DEVICE_MODE_UUID,
                                                                    NIMBLE_PROPERTY::READ |
                                                                    NIMBLE_PROPERTY::WRITE);

    startService();
    initializeCharacteristics();
}


void BLEInterface::initializeCharacteristics()
//Initializes characteristics and sets them to 0 for advertising.
{
    pAccelCharacteristic->setValue(0);
    pAccelCharacteristic->setCallbacks(&characteristicCallbacks);

    pRotationCharacteristic->setValue(0);
    pRotationCharacteristic->setCallbacks(&characteristicCallbacks);

    pHeadingCharacteristic->setValue(0);
    pHeadingCharacteristic->setCallbacks(&characteristicCallbacks);

    pTemperatureCharacteristic->setValue(0);
    pTemperatureCharacteristic->setCallbacks(&characteristicCallbacks);

    pBatteryLifeCharacteristic->setValue(0);
    pBatteryLifeCharacteristic->setCallbacks(&characteristicCallbacks);

    pDeviceModeCharacteristic->setValue(0);
    pDeviceModeCharacteristic->setCallbacks(&characteristicCallbacks);
}


void BLEInterface::startAdvertising()
{
    pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(IMU_SERVICE_UUID); 
    delay(500);

    //Setting scan response to false to preserve battery life
    pAdvertising->setScanResponse(false);
    pAdvertising->start();

}

void BLEInterface::startService()
{
    pTelemetryService->start();
    pDeviceInfoService->start();
}

void BLEInterface::streamAcceleration(uint64_t accelValue)
{
    pAccelCharacteristic->setValue(accelValue);
    pAccelCharacteristic->notify(true);
}

void BLEInterface::streamRotation(uint64_t rotationValue)
{
    pRotationCharacteristic->setValue(rotationValue);
    pRotationCharacteristic->notify(true);
}

void BLEInterface::streamHeading(uint64_t headingValue)
{
    pHeadingCharacteristic->setValue(headingValue);
    pHeadingCharacteristic->notify(true);
}