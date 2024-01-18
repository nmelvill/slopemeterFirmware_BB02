#include "skiSensor.h"
#include "common.h"




skiSensor::skiSensor(BLEInterface* pBLEInterface) : pBLEProcess(pBLEInterface)
{

    devicestate = starting;
    
    ICM20948 IMU;
    AK09916 MAG;
    esp32 controller;

    //devicestate = ready;
}


void skiSensor::initialize()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    
    delay(3000);

    IMU.connect();
    IMU.turnOn();
    
    //In order to use the magnetometer the I2C aux bus of the ICM20948 must be set to pass through mode
    IMU.enablePassThru();
    MAG.turnOn();

    MAG.readStatus2();      //Do I need this?
    

    delay(500);

    
}


void skiSensor::run()
{
//TODO: Needs to have a handler for messages

    if (pBLEProcess->isMessage()) {
        std::string rawMessage = getMessage();
        
        std::vector<std::string> commandVector = parseMessage(rawMessage);
        handleCommand(commandVector);
        }

    handleMode();
}


void skiSensor::handleMode()
//Checks the device mode and executes the correct method for the given mode
{
    switch (devicemode)
    {
        case IDLE: {
        //Does nothing during the main loop
            break; }
       
        case STREAM_RAW:{
        //Streams raw IMU values over BLE to the client
            streamRawValuesToBLE();
            break; }
    }
}


std::string skiSensor::getMessage()
//Gets first message in the queue from the BLE object and removes it from the queue.  Returns that message.
{
    std::string message = pBLEProcess->readMessage();

    ESP_LOGD(TAG,"Message %s, recieved", message.c_str());
    
    pBLEProcess->removeFirstMessage();

    
    return message;

}


std::vector<std::string> skiSensor::parseMessage(std::string message)
//Parses a message into the command and arguments using the <COMMAND> , <ARGUMENT1>, <ARGUMENT2>, ... format
{
    std::string delimiter = ",";
    size_t position = 0;
    std::string token;

    std::vector<std::string> commandVector;


    ESP_LOGD(TAG,"Message %s, ready to parse", message.c_str());
    ESP_LOGD(TAG,"Delimiter location in message: %zu" ,message.find(delimiter));

    if (message.find(delimiter) == std::string::npos) {
    //If the position for the delimiter doesn't exist then use the message as is as the first item in the command vector.
        
        commandVector.push_back(message);
        
    }
    
    else {
    //If the position of the delimiter does exist then loop through the string and add each substring before the delimiter to the vector in 
    //order.

        while((position = message.find(delimiter)) != std::string::npos) {
        
            std::string token = message.substr(0, position);

            commandVector.push_back(token);

            message.erase(0,position + delimiter.length());
        }
    }
    
    ESP_LOGD(TAG, "Message parsed to command: %s", commandVector[0].c_str());
    //ESP_LOGD(TAG,"message parsed");


    return commandVector;
}


void skiSensor::handleCommand(std::vector<std::string> commandVector)
//Using a conditional switch case that takes the first item of the commandVector and matches it with the appropriate function call
//Takes a vector with the first item the name of the function and the successive items the arguments, see devicemode enum
{
    std::string command = commandVector[0];
    
    int commandNum = commandMap[command];

    
    ESP_LOGD(TAG, "Command %s mapped to command Number: %d", command.c_str(), commandMap[command]);    

    switch (commandNum)
    {
        case STOP: {
        //The command stop will put the device into an IDLE mode and the run method will just break during the loop and will not execute anything    
            devicemode = IDLE;
            ESP_LOGD(TAG, "Device Mode changed to IDLE");
            break ;}
    
        case STREAM_RAW_VALUES: {
        //The command streamRaw will put the device into a STREAM_RAW mode which will execute the StreamRawValuesToBLE method during the loop   
            devicemode = STREAM_RAW;
            ESP_LOGD(TAG, "Device Mode changed to STREAM_RAW");
            break ;}
    }
}


void skiSensor::streamRawValuesToSerial()
{
    //maybe save this in an instance variable rather than creating two identical variables?
    std::vector<int16_t> Acceleration = getRawAcceleration();
    std::vector<int16_t> RotationalVelocity = getRawRotationalVelocity();
    std::vector<int16_t> Heading = getRawHeading();
    

    std::string type = "motionReading";
        
    std::map<std::string, int> payloadMap;
    payloadMap["rawAccelX"] = Acceleration[0];
    payloadMap["rawAccelY"] = Acceleration[1];
    payloadMap["rawAccelZ"] = Acceleration[2];
    payloadMap["rawGyroX"] = RotationalVelocity[0];
    payloadMap["rawGyroY"] = RotationalVelocity[1];
    payloadMap["rawGyroZ"] = RotationalVelocity[2];
    payloadMap["rawMagX"] = Heading[0];
    payloadMap["rawMagY"] = Heading[1];
    payloadMap["rawMagZ"] = Heading[2];

    message valueMessage(type, payloadMap, "0");
    valueMessage.buildMessage();
    valueMessage.send();

}


void setOutput(char outputtype)
{
    message::s_routerType = outputtype;
}


std::vector<int16_t> skiSensor::getRawAcceleration()
{
    IMU.readRawAcceleration();
    std::vector<int16_t> rawAcceleration = acceleration.getState(); 

    return rawAcceleration;
}


std::vector<int16_t> skiSensor::getRawRotationalVelocity()
{
    IMU.readRawRotationalVelocity();
    std::vector<int16_t> rawRotationalVelocity = rotationalVelocity.getState(); 

    return rawRotationalVelocity;
}


std::vector<int16_t> skiSensor::getRawHeading()
{
    MAG.readRawHeading();
    std::vector<int16_t> rawHeading = heading.getState(); 

    return rawHeading;
}

void skiSensor::streamRawValuesToBLE()
//Converts an array of 3 axis IMU values into 3 separate 64bit integers and streams them over BLE
{

    //Combine separate values into one 64bit integer to send over BLE
    uint64_t Acceration3Axis = serializeVector(getRawAcceleration());
    uint64_t Rotation3Axis = serializeVector(getRawRotationalVelocity());
    uint64_t Heading3Axis = serializeVector(getRawHeading());

    //Stream (notify) values over BLE
    pBLEProcess->streamAcceleration(Acceration3Axis);
    pBLEProcess->streamRotation(Rotation3Axis);
    pBLEProcess->streamHeading(Heading3Axis);
}


uint64_t skiSensor::serializeVector(std::vector<int16_t> inputVector)
//Converts a vector of three 16bit values to a 64bit unsigned integer in Big Endian Format using signed magnitude
/* 
Output Example: 0x0000FA70CE6828D0
N/A   x      y      z
0000  FA70   CE68   28D0
0000 -1424  -12696  10448

*/
{
    #define FIRST_BIT_MASK 0x0000FFFF00000000
    #define SECOND_BIT_MASK 0X00000000FFFF0000
    #define THIRD_BIT_MASK 0x000000000000FFFF

    uint64_t outputInteger = 0;

    outputInteger |=  ((uint64_t) inputVector[0] << 32) & FIRST_BIT_MASK |  
                        ((uint64_t) inputVector[1] << 16) & SECOND_BIT_MASK |  
                        (uint64_t) inputVector[2] & THIRD_BIT_MASK;

    return outputInteger;

}