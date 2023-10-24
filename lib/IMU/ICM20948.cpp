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

uint8_t Register::Write(uint8_t writeValue, bool printValues /*=true*/) 
//Writes a values to a specific register, only works for one byte (one register) use combo register for multiple bytes/registers
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                           
    Wire.write(writeValue);
    uint8_t int_status = Wire.endTransmission(true); 

    std::string status = std::to_string(int_status);
    
    std::string type = "registerWrite";
    
    std::map<std::string, int> payloadMap;
    payloadMap["device"] = deviceAddress;
    payloadMap["register"] = address;
    payloadMap["value"] = writeValue;

    payload messagePayload(payloadMap);
    message writeStatusMessage(type, messagePayload.build(), status);
    writeStatusMessage.buildMessage(); 

    delay(500);
    return int_status;                        
}


uint8_t Register::Read(bool printValues /*=true*/)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                            
    uint8_t int_status = Wire.endTransmission(true); 
    Wire.requestFrom(deviceAddress, 1);          

    std::string status = std::to_string(int_status);
    std::string type = "registerRead";
    byte readValue = Wire.read(); 
    
    std::map<std::string, int> payloadMap;
    payloadMap["device"] = deviceAddress;
    payloadMap["register"] = address;
    payloadMap["value"] = readValue;

    payload messagePayload(payloadMap);
    message writeStatusMessage(type, messagePayload.build(), status);
    writeStatusMessage.buildMessage(); 
    
    Serial.println(readValue, BIN);

    return int_status;                                  
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
    uint8_t status = Wire.endTransmission(true); 
    Wire.requestFrom(deviceAddress, regSize);         
    
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


void ICM20948::switchUserBank(int bank)
{
    uint8_t writeValue;
    switch(bank)
    {
        case 0:
            writeValue = 0b00000000;
            break;
        case 1:
            writeValue = 0b00010000;
            break;
        case 2:
            writeValue = 0b00100000;
            break;
        case 3:
            writeValue = 0b00110000;
            break;
        default:
            break;
    }

    userBank.Write(writeValue);

}

//AK09916 class
AK09916::AK09916()
    :isConnected(false)
{

}

void AK09916::turnOn()
{
    //Wake up and reset magnetometer
    control3.Write(0b00000001);
    //Put magnetometer in continuous read mode
    control2.Write(0b00000010);
    


}

