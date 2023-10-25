#ifndef MESSAGE_H
#define MESSAGE_H

#include "Arduino.h"
#include <string>
#include "ArduinoJson.h"
#include <map>

class message
{
    public:
    message(const std::string type, std::map<std::string, int> payloadValues, std::string status = "N/A");
    void buildMessage();

    private:
    std::string type;
    std::string payloadString;
    std::string status;
    DynamicJsonDocument jsonMessage {1024};
    //DynamicJsonDocument payload {1024};
    std::map<std::string, int> payloadValues;
    void buildPayload();

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
