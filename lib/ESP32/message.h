#ifndef MESSAGE_H
#define MESSAGE_H

#include "Arduino.h"
#include <string>
#include "ArduinoJson.h"
#include <map>

class message
{
    public:
    message(const std::string type, std::string payload, std::string status = "N/A");
    void buildMessage();

    private:
    std::string type;
    std::string payload;
    std::string status;
    DynamicJsonDocument jsonMessage {1024};

};

class payload
{
    public:
    payload(std::map<std::string, int> values);
    std::string build();

    private:
    void buildJSON();
    std::map<std::string, int> values;
    std::string jsonString;
    DynamicJsonDocument payloadValues {64};

};

class messageRouter
{
    public:
    messageRouter(const std::string routerType);
    std::string handleInput(std::string input);

    private:
    const std::string routerType;

};

#endif
