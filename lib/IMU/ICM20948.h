#ifndef ICM20948_H
#define ICM20948_H

#include "Arduino.h"
#include <string>
#include "property.h"
#include "registers.h"

class Register
{
    public:
    Register(const uint8_t I2CAddress, const uint8_t regAddress);
    bool Write(uint8_t writeValue);
    uint8_t Read(); 

    // accessors
    uint8_t getDeviceAddress() {return deviceAddress;}
    uint8_t getAddress() {return address;}
    uint8_t getReadStatus() { return readStatus;}
    uint8_t getWriteStatus() { return writeStatus;}
    
    private:
    const uint8_t deviceAddress;
    const uint8_t address;
    uint8_t value;
    uint8_t readStatus;
    uint8_t writeStatus;
};


class ComboRegister
{
    public:
    ComboRegister(const uint8_t I2CAddress, const uint8_t regAddress, uint8_t readSize);
    uint8_t Read();
    uint8_t value;
    uint8_t regSize;

    //accessors
    uint8_t getDeviceAddress() {return deviceAddress;}
    uint8_t getAddress() {return address;}
    uint8_t getReadStatus() { return readStatus;}
    uint8_t getWriteStatus() { return writeStatus;}
    
    private:
    const uint8_t deviceAddress;
    const uint8_t address;
    uint8_t readStatus;
    uint8_t writeStatus;
};



class ICM20948
{
    public:
    ICM20948();
    void connect();
    void turnOn();

    //accessors
    bool getIsConnected() { return isConnected;}
    std::vector<int> readRotationalVelocity();
    std::vector<int> readAccleration();
    std::vector<int> readHeading();
    
    
    private:
    uint8_t ICMAddress = 0x69;
    bool isConnected;
    ComboRegister accelerationBank {ICMAddress, 0x2D, 6};
    ComboRegister gyroBank {ICMAddress, 0x33, 6};
    ComboRegister magnometerBank {ICMAddress, 0x11, 6};
    MotionState acceleration;
    MotionState rotationalVelocity;
    MotionState heading;

    void enableAxes();
    void resetUsrCtrl();

};




#endif