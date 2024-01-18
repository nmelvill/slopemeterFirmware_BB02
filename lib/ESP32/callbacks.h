#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "Arduino.h"
#include "NimBLEDevice.h"
#include "NimBLEAttValue.h"


//forward declaration to avoid circular dependency
class BLEInterface;


class CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
    
    
public:
    void onRead(NimBLECharacteristic* pCharacteristic);
    void onWrite(NimBLECharacteristic* pCharacteristic);
    void addInterface(BLEInterface* pInterfaceObject);

private:

    BLEInterface *pBLEInterface;
};


#endif