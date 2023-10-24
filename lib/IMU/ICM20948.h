#ifndef ICM20948_H
#define ICM20948_H

#include "Arduino.h"
#include <string>
#include "registers.h"
#include "message.h"
#include "ArduinoJson.h"

class Register
{
    public:
    Register(const uint8_t I2CAddress, const uint8_t regAddress);
    uint8_t Write(uint8_t writeValue, bool printValues = true);
    uint8_t Read(bool printValues = true); 

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
    void readAcceleration() {accelerationBank.Read();}
    void readRotationalVelocity() {gyroBank.Read();}

    //modifiers
    void switchUserBank(int bank);
    void enablePassThru() {intPinConfig.Write(1<<1);}
    
    
    private:
    uint8_t ICMAddress = 0x69;

    bool isConnected;
    ComboRegister accelerationBank {ICMAddress, 0x2D, 6};
    ComboRegister gyroBank {ICMAddress, 0x33, 6};

    Register powerManagement1 {ICMAddress, PWR_MGMT_1};
    Register powerManagement2 {ICMAddress, PWR_MGMT_2};
    Register intPinConfig {ICMAddress, INT_PIN_CFG};
    Register userControl {ICMAddress, USR_CTRL};
    Register userBank {ICMAddress, REG_BANK_SEL};
    
    
    

};

class AK09916
{
    public:
    AK09916();
    void turnOn();

    //accessors
    void readStatus1(bool printValues) {status1.Read(printValues);}
    void readStatus2() {status2.Read();}
    void readHeading() {magnetometerBank.Read();}

    private:
    uint8_t MagAddress = 0x0C;
    bool isConnected;

    //8 Registers instead of 6 to ensure that status2 is also read which is required
    ComboRegister magnetometerBank {MagAddress, HXL, 8};
    //MotionState heading;

    Register control2 {MagAddress, CNTL2};
    Register control3 {MagAddress, CNTL3};
    Register status2 {MagAddress, ST2};
    Register status1 {MagAddress, ST1};
};

#endif