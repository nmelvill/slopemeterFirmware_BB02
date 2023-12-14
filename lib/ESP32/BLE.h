//Holds all information for using Bluetooth Low Energy (BLE) actual BLE setup is held in the esp32 class

#ifndef BLE_H
#define BLE_H

#include "Arduino.h"
#include "NimBLEDevice.h"


#define IMU_SERVICE_UUID                    "5d966f57-d014-4744-8704-f256701f0409"
#define DEVICE_INFO_SERVICE_UUID            "355b31e2-1613-4603-aa15-46b2bd179901"
#define ACCELERATION_CHARACTERISTIC_UUID    "569d314e-9499-4e4c-9082-39354e4f5f58"
#define ROATION_CHARACTERISTIC_UUID         "f59c06f3-e55a-403c-bef7-d4b9fbb00c7f"
#define HEADING_CHARACTERISTIC_UUID         "9c71c184-8788-4c25-8368-26ed1e87cd94"
#define TEMPERATURE_CHARACTERISTIC_UUID     "16bb58ab-5b69-4fc6-9bf5-a6794031e6cc"
#define BATTERY_LIFE_CHARACTERISTIC_UUID    "1a9e87a5-a475-44ca-9872-cb18779fad64"



class BLE
{
    public:
    BLE();
    void initialize();
    void startService();
    void startAdvertising();
    void streamAcceleration(uint64_t accelValue);
    void streamRotation(uint64_t rotationValue);
    void streamHeading(uint64_t headingValue);

    private:
    NimBLEServer *pServer;
    NimBLEService *pTelemetryService;
    NimBLEService *pDeviceInfoService;
    NimBLECharacteristic *pAccelCharacteristic;
    NimBLECharacteristic *pRotationCharacteristic;
    NimBLECharacteristic *pHeadingCharacteristic;
    NimBLECharacteristic *pTemperatureCharacteristic;
    NimBLECharacteristic *pBatteryLifeCharacteristic;
    NimBLEAdvertising *pAdvertising;

    void initializeCharacteristics();

};




#endif