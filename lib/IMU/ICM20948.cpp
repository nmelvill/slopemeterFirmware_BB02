#include "ICM20948.h"
#include "Wire.h"
#include <exception>
#include <string>

//Register class
Register::Register(const uint8_t I2CAddress, const uint8_t regAddress, const char userBank /*=0*/)      //Constructor
    : value(0x00),                                                          //member initializer list
    deviceAddress(I2CAddress), 
    address(regAddress),
    userbank(userBank) 
{
}

uint8_t Register::Write(uint8_t writeValue, bool printValues /*=true*/) 
//Writes a values to a specific register, only works for one byte (one register) use combo register for multiple bytes/registers
{
    switchUserBank(userbank);

    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                           
    Wire.write(writeValue);
    uint8_t int_status = Wire.endTransmission(true); 

    std::string status = std::to_string(int_status);
    std::string type = "registerWrite";
    
    if (printValues){
        std::map<std::string, int> payloadMap;
       
        payloadMap["device"] = deviceAddress;
        payloadMap["register"] = address;
        payloadMap["value"] = writeValue;
        
        message writeStatusMessage(type, payloadMap, status);
        writeStatusMessage.buildMessage(); 
    }

    return int_status;                        
}


uint8_t Register::Read(bool printValues /*=true*/)
{
    switchUserBank(userbank);
    
    Wire.beginTransmission(deviceAddress);
    Wire.write(address);                            
    uint8_t int_status = Wire.endTransmission(true); 
    Wire.requestFrom(deviceAddress, 1);          

    std::string status = std::to_string(int_status);
    std::string type = "registerRead";
    byte readValue = Wire.read(); 
    
    if (printValues){
        
        std::map<std::string, int> payloadMap;
        payloadMap["device"] = deviceAddress;
        payloadMap["register"] = address;
        payloadMap["value"] = readValue;

        message writeStatusMessage(type, payloadMap, status);
        writeStatusMessage.buildMessage(); 
    }
    return readValue;                                  
}


void Register::switchUserBank(char bank)
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

    Wire.beginTransmission(deviceAddress);
    Wire.write(REG_BANK_SEL); 
    Wire.write(writeValue);                       
    uint8_t int_status = Wire.endTransmission(true);
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

    delay(100);
    
    //Reset, select clock source and keep in sleep mode.
    powerManagement1.Write(0b11000001);
    delay(100);
    
    //Wake up unit and choose best available clock source
    powerManagement1.Write(0x01);
    delay(100);
    
    //Aligns the output data rate
    odrAlign.Write(0x01);
    delay(100);

    //Initialize gyrometer configuration
    setGyroConfig();

    powerManagement2.Write(0x00);
    delay(100);
    userControl.Write(0x00);
}


void ICM20948::setGyroConfig(int samplerateDivider/*=0*/,
                    int fullScale/*=250*/, 
                    bool lowpass/*=false*/, 
                    uint8_t lowpassConfig/*=0x00*/,
                    bool selftestEnable/*=false*/,
                    int averaging/*=1 */)
{
    
    //Set sample rate of the gyroscope 1.1khz/(1+samplerateDivider) p59
    if (samplerateDivider > 256)
    {
        //set to maximum sample rate
        //TODO: throw error here if sample rate divider exceeds 256 or is a decimal
        samplerateDivider = 256;
    }
    
    gyroSampleRateDiv.Write(samplerateDivider);




}

void ICM20948::setGyroRange(int fullscale/*=250*/)
{

}

//AK09916 class
AK09916::AK09916()
    :isConnected(false)
{
}


void AK09916::turnOn()
{
    //Wake up and reset magnetometer
    delay(500);
    control3.Write(0b00000001);
    //Put magnetometer in continuous read mode
    delay(500);
    control2.Write(0b00000010);
    
}


void AK09916::readRawHeading()
{  
    bool dataReady;
    
    dataReady = bitRead(status1.Read(false),0);
    
    if (dataReady == 1){
        magnetometerBank.Read();
    }
}
