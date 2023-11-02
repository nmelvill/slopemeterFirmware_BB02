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
    Register(const uint8_t I2CAddress, const uint8_t regAddress, const char userBank=0);
    uint8_t Write(uint8_t writeValue, bool printValues = true);
    uint8_t Read(bool printValues = true); 

    // accessors
    uint8_t getDeviceAddress() {return deviceAddress;}
    uint8_t getAddress() {return address;}
    uint8_t getReadStatus() { return readStatus;}
    uint8_t getWriteStatus() { return writeStatus;}

    // modifiers
    void switchUserBank(char bank);
    
    private:
    const uint8_t deviceAddress;
    const uint8_t address;
    uint8_t value;
    uint8_t readStatus;
    uint8_t writeStatus;
    char userbank;
};


class ComboRegister
{
    public:
    ComboRegister(const uint8_t I2CAddress, const uint8_t regAddress, uint8_t registerSize);
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
    
    //initializers
    void connect();
    void turnOn();
    void enablePassThru() {intPinConfig.Write(1<<1);}
    void setGyroConfig(int samplerateDivider=0,
                    int fullScale=250, 
                    bool lowpass=false, 
                    uint8_t lowpassConfig=0x00,
                    bool selftestEnable=false,
                    int averaging=1 );

    void setAccelConfig(int samplerateDivider=0,
                int fullScale=2, 
                bool lowpass=false, 
                uint8_t lowpassConfig=0x00,
                bool selftestEnable=false,
                int averaging=1 );

    //accessors
    bool getIsConnected() { return isConnected;}
    void readRawAcceleration() {accelerationBank.Read();}
    void readRawRotationalVelocity() {gyroBank.Read();}
    

    //modifiers  
    void setGyroRange(int fullscale=250);
    void setGyroLowPassConfig(uint8_t lowPassParam = 0);
    void setGyroLowPasFilter(bool lowPass = false);
    void setGyroSelfTest(bool selfTest = false);
    void setGyroAveraging(uint8_t averagingParam = 0);

    void setAccelRange(int fullscale=2);
    void setAccelLowPassConfig(uint8_t lowPassParam = 0);
    void setAccelLowPasFilter(bool lowPass = false);
    void setAccelSelfTest(bool selfTest = false);
    void setAccelAveraging(uint8_t averagingParam = 0);

    void setUserBank(uint8_t userbank);
    
    
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
    Register odrAlign {ICMAddress, ODR_ALIGN_EN, 2};
    Register gyroSampleRateDiv {ICMAddress, GYRO_SMPLRT_DIV, 2};
    Register gyrcoConfig1 {ICMAddress, GYRO_CONFIG_1, 2};
    Register gyrcoConfig2 {ICMAddress, GYRO_CONFIG_2, 2};
    Register accelConfig1 {ICMAddress, ACCEL_CONFIG_1, 2};
    Register accelConfig2 {ICMAddress, ACCEL_CONFIG_2, 2};


    int maskByte(uint8_t initialValue, char maskLocation, char maskSize, uint8_t writeValue);
    
    
    

};

class AK09916
{
    public:
    AK09916();
    void turnOn();

    //accessors
    void readStatus1(bool printValues) {status1.Read(printValues);}
    void readStatus2() {status2.Read();}
    void readRawHeading();

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