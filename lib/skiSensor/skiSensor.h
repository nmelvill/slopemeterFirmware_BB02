
#ifndef SKISENSOR_H
#define SKISENSOR_H


#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
//static const char* TAG = "BB02";

#include <Arduino.h>
#include "Wire.h"
#include "ble.h"
#include "ICM20948.h"
#include "state.h"
#include <array>
#include "ESP32.h"
#include "esp_log.h"
#include <unordered_map>
#include <functional>



class skiSensor
//Main hub and executive of the skiSensor library 
{
public:
      
    skiSensor(BLEInterface* pBLEInterface);
    
    void initialize();
    void run();
    void streamRawValuesToSerial();
    void streamRawValuesToBLE();
    void setOutput(char outputType = 0){message::s_routerType = outputType;}  //0 = Serial, 1= BLE, 2= Both
    std::string getMessage();
    void handleMessage(std::string message);
    std::vector<std::string> parseMessage(std::string message);
    void handleCommand(std::vector<std::string> commandVector);
    void handleMode();
    int getMode() {return devicemode;}
    int getState() {return devicestate;}


    std::vector<int16_t> getRawAcceleration();
    std::vector<int16_t> getRawRotationalVelocity();
    std::vector<int16_t> getRawHeading();
    

    enum deviceStateList {
    //This is not a user-modifiable value and represents the internal state of the device
        starting,
        ready,
        running,
        error
    };

    enum deviceModeList {
    //These represent the various modes that the device can be in, after checking for a new message 
    //the device will run methods depending on the mode that it is in.
        IDLE,
        STREAM_RAW,
        STREAM_VALUES
    };

    enum commandList {
    //This is a list of the potential commands as an enum to be used in a switch/case conditional
    //The commandMap below maps these enums to string commands sent from the client
        STOP,
        STREAM_RAW_VALUES

    };
    
    std::unordered_map<std::string, int> commandMap = {
    //Shim that maps the string commands from the client to enum values for use in a switch/case conditional in the handleCommand method
        {"stop", STOP},
        {"streamRaw", STREAM_RAW_VALUES}

    };

private:
    
    ICM20948 IMU;
    AK09916 MAG;
    esp32 controller;
    BLEInterface *pBLEProcess;

    MotionState acceleration{true, 6};
    MotionState rotationalVelocity{true, 6};
    MotionState heading{false, 6};

    uint64_t serializeVector(std::vector<int16_t> inputVector);

    int devicemode;
    int devicestate;
    
    

    //methods

};

#endif