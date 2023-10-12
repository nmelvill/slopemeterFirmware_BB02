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

uint8_t Register::Write(uint8_t writeValue)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                           
    Wire.write(writeValue);
    uint8_t status = Wire.endTransmission(true); 
    
    return status;                        
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
    Serial.print("Power Management 1 Status ");
    Serial.println(powerManagement1.Write(0x00));
    delay(1000);
    Serial.print("Power Management 2 Status ");
    Serial.println(powerManagement2.Write(0x00));
    delay(1000);
    Serial.print("User Control Status ");
    Serial.println(userControl.Write(0x00));
    delay(1000);
    Serial.println();
    Serial.println("ICM20948 Initialized");
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

//AK09916 class
AK09916::AK09916()
    :isConnected(false)
{

}

void AK09916::turnOn()
{
    //Wake up magnetometer
    control2.Write(0b00000010);
}

std::vector<int> AK09916::readHeading()
{
    magnetometerBank.Read();
    return heading.getState();
}
