#include "BLE.h"
#include "common.h"
#include "callbacks.h"


/** Handler class for characteristic actions */



static CharacteristicCallbacks characteristicCallbacks;


BLEInterface::BLEInterface() = default;


void BLEInterface::initialize()
{
    NimBLEDevice::init("BB02");

    pServer = NimBLEDevice::createServer();
    pTelemetryService = pServer->createService(IMU_SERVICE_UUID);
    pDeviceInfoService = pServer->createService(DEVICE_INFO_SERVICE_UUID);
    
    characteristicCallbacks.addInterface(this);
    
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

    pControlPointCharacteristic = pDeviceInfoService->createCharacteristic(CONTROL_POINT_CHARACTERISTIC_UUID,
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

    pControlPointCharacteristic->setValue(0);
    pControlPointCharacteristic->setCallbacks(&characteristicCallbacks);
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

void BLEInterface::addMessage(std::string message) 

{
    messageQueue.push(message);

    ESP_LOGD(TAG, "Message:  %s, added to the top of the message queue", message.c_str());
}


std::string BLEInterface::readMessage() 

{
    std::string message = messageQueue.front();
    
    ESP_LOGD(TAG, "Message at the top of the queue: %s", message.c_str());
    
    return message;
}

int BLEInterface::removeFirstMessage() 

{
    messageQueue.pop();

    ESP_LOGD(TAG, "First message removed from the queue");

    return 1;

}

bool BLEInterface::isMessage() {
//Checks if there is a message in the message queue

    if (messageQueue.empty())
        
        return false;
    
    else
        ESP_LOGD(TAG, "%zu message(s) are available in the queue", messageQueue.size());
        
        return true;
        
        
}