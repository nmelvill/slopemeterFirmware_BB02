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
    Wire.write(address);                           
    Wire.write(writeValue);
    uint8_t status = Wire.endTransmission(false); 
    
    return true;                        
}


uint8_t Register::Read()
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                            
    uint8_t status = Wire.endTransmission(false); 
    Wire.requestFrom(deviceAddress, 1, 1);                   
    
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
    uint8_t status = Wire.endTransmission(false); 
    Wire.requestFrom(deviceAddress, regSize, 1);              
    
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
    delay(100);
    isConnected = true;
}

void ICM20948::turnOn()
{
    //Wake up accelerometer and gyroscope
    powerManagement1.Write(0x00);
    powerManagement2.Write(0x00);
    userControl.Write(0x00);
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

