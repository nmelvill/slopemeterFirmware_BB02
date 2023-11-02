#include "ICM20948.h"
#include "Wire.h"
#include <exception>
#include <string>
#include <bitset>

//Register class
Register::Register(const uint8_t I2CAddress, const uint8_t regAddress, const char userBank /*=0*/) 
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
ComboRegister::ComboRegister(const uint8_t I2CAddress, const uint8_t regAddress, const uint8_t registerSize)
    : value(0x00),                                                         
    deviceAddress(I2CAddress), 
    address(regAddress),
    regSize(registerSize)
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
    
    //Enable all accelerometer axes and gyroscope axes
    powerManagement2.Write(0x00);
    delay(100);

    //Aligns the output data rate
    odrAlign.Write(0x01);
    delay(100);

    //Initialize gyrometer configuration
    setGyroConfig();
    delay(100);

    //Initialize accelerometer configuration
    setAccelConfig();
    delay(100);

    //Disables DMP, FIFO and I2C master modes see p36 of the datasheet.
    userControl.Write(0x00);
    delay(100);

    userBank.switchUserBank(0);
}


void ICM20948::setGyroConfig(int samplerateDivider/*=0*/,
                    int fullScale/*=250*/, //See pg 59 of datasheet, can be 250, 500, 1000 or 2000
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

    setGyroRange();
    setGyroLowPassConfig();
    setGyroLowPasFilter();
    setGyroSelfTest();
    setGyroAveraging();


}

void ICM20948::setGyroRange(int fullScale/*=250*/)
//Set the full scale of degrees per second the gyroscope will read before clipping
/*  250 DPS
    500 DPS
    1000 DPS
    2000 */
{
    
    uint8_t fullScaleSwitch;
    switch (fullScale)
    {
    case 250:
        fullScaleSwitch = 0b00000000;
        break;
    case 500:
        fullScaleSwitch = 0b00000010;
        break; 
    case 1000:
        fullScaleSwitch = 0b00000100;
        break;          
    case 2000:
        fullScaleSwitch = 0b00000110;
        break;         
    default:
        //TODO Write error here if not a valid full scale type
        break;
    }

    uint8_t currentValue(gyrcoConfig1.Read(false));

    uint8_t writeValue = maskByte(currentValue, 2, 2, fullScaleSwitch);

    gyrcoConfig1.Write(writeValue);
}


void ICM20948::setGyroLowPassConfig(uint8_t lowPassParam)
/*Sets the low pass configuration for the gyroscope see pg 60 table 16

GYRO_FCHOICE    GYRO_DLPFCFG    3DB BW[HZ]      NBW[HZ]     RATE [HZ]
0               x               12106           12316       9000
1               0               196.6           229.8       1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               1               151.8           187.6       1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               2               119.5           154.3       1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               3               51.2            73.3        1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               4               23.9            35.9        1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               5               11.6            17.8        1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               6               5.7             8.9         1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               7               361.4           376.5       1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255*/

{
    lowPassParam <<= 3;

    uint8_t currentValue(gyrcoConfig1.Read(false));
    uint8_t writeValue = maskByte(currentValue, 5, 3, lowPassParam);

    gyrcoConfig1.Write(writeValue);    

}


void ICM20948::setGyroLowPasFilter(bool lowPass)
//Turns the low pass filter on the gyroscope on or off
{
    uint8_t currentValue(gyrcoConfig1.Read(false));
    uint8_t writeValue = maskByte(currentValue, 0, 1, lowPass);

    gyrcoConfig1.Write(writeValue);
}


void ICM20948::setGyroSelfTest(bool selfTest)
//Turns the self test on or off for all three gyroscope axes
{
    uint8_t selfTestBin;

    if(selfTest)
    {
        selfTestBin = 0b00111000;
    }
    else
    { 
        selfTestBin = 0b00000000;
    }
    
    uint8_t currentValue(gyrcoConfig2.Read(false));  
    uint8_t writeValue = maskByte(currentValue, 5, 3, selfTestBin);

    gyrcoConfig2.Write(writeValue);

}


void ICM20948::setGyroAveraging(uint8_t averagingParam)
//Sets Averaging filter and configuration settings for low-power mode.  Pg 60 Table 17
/*  0: 1x averaging.
    1: 2x averaging.
    2: 4x averaging.
    3: 8x averaging.
    4: 16x averaging.
    5: 32x averaging.
    6: 64x averaging.
    7: 128x averaging.*/
{
    
    uint8_t currentValue(gyrcoConfig2.Read(false));  
    uint8_t writeValue = maskByte(currentValue, 2, 3, averagingParam);

    gyrcoConfig2.Write(writeValue);
}


void ICM20948::setAccelConfig(int samplerateDivider/*=0*/,
                    int fullScale/*=2*/, //See pg 64 of datasheet, can be 2g, 4g, 8g, or 16g
                    bool lowpass/*=false*/, 
                    uint8_t lowpassConfig/*=0x00*/,
                    bool selftestEnable/*=false*/,
                    int averaging/*=1 */)
{
    
    ////Set sample rate of the accelerometer 1.1khz/(1+samplerateDivider) p59
    //if (samplerateDivider > 256)
    //{
    //    //set to maximum sample rate
    //    //TODO: throw error here if sample rate divider exceeds 256 or is a decimal
    //    samplerateDivider = 256;
    //}
    //
    //gyroSampleRateDiv.Write(samplerateDivider);

    setAccelRange();
    setAccelLowPassConfig();
    setAccelLowPasFilter();
    setAccelSelfTest();
    setAccelAveraging();


}


void ICM20948::setAccelRange(int fullScale/*=250*/)
//Set the full scale of gs (acceleration of earths gravity) that the accelerometer will read before clipping
/*  2g
    4g
    8g
    16g */
{
    
    uint8_t fullScaleSwitch;
    switch (fullScale)
    {
    case 2:
        fullScaleSwitch = 0b00000000;
        break;
    case 4:
        fullScaleSwitch = 0b00000010;
        break; 
    case 8:
        fullScaleSwitch = 0b00000100;
        break;          
    case 16:
        fullScaleSwitch = 0b00000110;
        break;         
    default:
        //TODO Write error here if not a valid full scale type
        break;
    }

    uint8_t currentValue(accelConfig1.Read(false));

    uint8_t writeValue = maskByte(currentValue, 2, 2, fullScaleSwitch);

    accelConfig1.Write(writeValue);
}


void ICM20948::setAccelLowPassConfig(uint8_t lowPassParam)
/*Sets the low pass configuration for the accelerometer see pg 64 table 18

ACCEL_FCHOICE   ACCEL_DLPFCFG   3DB BW[HZ]      NBW[HZ]     RATE [HZ]
0               x               1209            1248        4500
1               0               246             265         1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               1               246             265         1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               2               111.4           136         1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               3               50.4            68.8        1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               4               23.9            34.4        1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               5               11.5            17          1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               6               5.7             8.3         1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255
1               7               473             499         1125/(1+GYRO_SMPLRT_DIV)Hz where GYRO_SMPLRT_DIV is 0, 1, 2,…255*/

{
    lowPassParam <<= 3;

    uint8_t currentValue(accelConfig1.Read(false));
    uint8_t writeValue = maskByte(currentValue, 5, 3, lowPassParam);

    accelConfig1.Write(writeValue);    

}


void ICM20948::setAccelLowPasFilter(bool lowPass)
//Turns the low pass filter on the accelerometer on or off
{
    uint8_t currentValue(accelConfig1.Read(false));
    uint8_t writeValue = maskByte(currentValue, 0, 1, lowPass);

    accelConfig1.Write(writeValue);
}


void ICM20948::setAccelSelfTest(bool selfTest)
//Turns the self test on or off for all three accelerometer axes
{
    uint8_t selfTestBin;

    if(selfTest)
    {
        selfTestBin = 0b00011100;
    }
    else
    { 
        selfTestBin = 0b00000000;
    }
    
    uint8_t currentValue(accelConfig2.Read(false));  
    uint8_t writeValue = maskByte(currentValue, 4, 3, selfTestBin);

    accelConfig2.Write(writeValue);

}


void ICM20948::setAccelAveraging(uint8_t averagingParam)
//Sets Averaging filter and configuration settings for low-power mode.  Pg 65 Table 19
/*  0: Average 1 or 4 samples depending on ACCEL_FCHOICE (see Table 19).
    1: Averages 8 Samples.
    2: Averages 16 Samples.
    3: Averages 32 Samples.*/
{
    
    uint8_t currentValue(accelConfig2.Read(false));  
    uint8_t writeValue = maskByte(currentValue, 2, 3, averagingParam);

    accelConfig2.Write(writeValue);
}


int ICM20948::maskByte(uint8_t initialValue, char maskLocation, char maskSize, uint8_t writeValue)
//Masks a read byte from the sensor to apply a specific values to certain bits while retaining the rest
{
    uint8_t initialMask = 0b00000001;
    uint8_t flipmask = 0b00000000;
    uint8_t mask = 0b11111111;
    uint8_t maskedByte;
    
    for (int i = maskLocation; i >= (maskLocation - maskSize + 1 ); i--)
    {
        uint8_t tempMask = initialMask << i;
        flipmask = flipmask | tempMask;
    }

    mask = mask ^ flipmask;
    
    return (mask & initialValue) | writeValue;
}


//AK09916 class
AK09916::AK09916()
    :isConnected(false)
{
}


void AK09916::turnOn()
{
    //Wake up and reset magnetometer
    delay(100);
    control3.Write(0b00000001);
    //Put magnetometer in continuous read mode
    delay(100);
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
