#include "ICM20948.h"
#include "Wire.h"
#include <exception>
#include <string>


//Register class
Register::Register(const uint8_t I2CAddress, const uint8_t regAddress)      //Constructor
    : value(0x00),                                                          //member initializer list
    deviceAddress(I2CAddress), 
    address(regAddress) 
{
}

bool Register::Write(uint8_t writeValue)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                            //Moves the pointer to the address
    Wire.write(writeValue);
    //End transmission actually is the command that takes whatever is on the register and moves it to the I2C buffer on the processing chip
    uint8_t status = Wire.endTransmission(false); 
    
    return true;                        //Returns true if successful 
}


uint8_t Register::Read()
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                            //Moves the pointer to the address
    //End transmission actually is the command that takes whatever is on the register and moves it to the I2C buffer on the processing chip
    uint8_t status = Wire.endTransmission(false); 
    
    //Go get the values that have been placed on the register and put them on the I2C read buffer
    Wire.requestFrom(deviceAddress, 1, true);                   
    
    return status;                                  //Returns Success if successful
}


//Combo Register Class
ComboRegister::ComboRegister(const uint8_t I2CAddress, const uint8_t regAddress, const uint8_t readSize)
    : value(0x00),                                                         
    deviceAddress(I2CAddress), 
    address(regAddress),
    regSize(readSize)
{
}

uint8_t ComboRegister::Read()
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                            //Moves the pointer to the address
    //End transmission actually is the command that takes whatever is on the register and moves it to the I2C buffer on the processing chip
    uint8_t status = Wire.endTransmission(false); 
    
    //Go get the values that have been placed on the register and put them on the I2C read buffer
    Wire.requestFrom(deviceAddress, regSize, true);               
    
    return status;                                  //Returns Success if successful
}


//ICM20948 class
ICM20948::ICM20948()                         //Constructor
    : isConnected(false)                  //member initializers list
{ 
}

void ICM20948::connect()
{
    Wire.begin(); 
    Wire.beginTransmission(ICM20948::ICMAddress);
    delay(100);
    isConnected = true;
}

void ICM20948::turnOn()
{
    Wire.write(0x06);              //PWR_MGMT_1 Register to wake unit up.
    Wire.write(0x00);
    Wire.endTransmission(true);    // Data doesn't transfer until endTransmission has been run
}

std::vector<int> ICM20948::readAccleration()
{
    accelerationBank.Read();
    return acceleration.getState();  
}

std::vector<int> ICM20948::readRotationalVelocity()
{
    gyroBank.Read();
    return rotationalVelocity.getState();
}

