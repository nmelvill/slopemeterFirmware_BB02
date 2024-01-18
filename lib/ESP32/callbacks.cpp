#include "callbacks.h"
#include "esp_log.h"
#include "common.h"
#include "ble.h"



void CharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic){
    //ESP_LOGI(TAG, pCharacteristic->getUUID().toString().c_str());
    //ESP_LOGI(TAG,": onRead(), value: ");
    //ESP_LOGI(TAG, pCharacteristic->getValue().c_str());
};

void CharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    //ESP_LOGI(TAG, pCharacteristic->getUUID().toString().c_str());
    std::string writeValue = pCharacteristic->getValue();

    pBLEInterface ->addMessage(writeValue);
    
    ESP_LOGI(TAG, "charateristic UUID %s, write value: %s", pCharacteristic->getUUID().toString().c_str(), writeValue.c_str());
};

void CharacteristicCallbacks::addInterface(BLEInterface* pInterfaceObject) {
    pBLEInterface = pInterfaceObject;
}