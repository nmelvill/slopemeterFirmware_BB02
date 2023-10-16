#ifndef MESSAGE_H
#define MESSAGE_H

#include "Arduino.h"
#include <string>
#include "ArduinoJson.h"

class message
{
    public:
    message(const std::string type, std::string payload, std::string status = "N/A");
    void getMessage();

    private:
    std::string type;
    std::string payload;
    std::string status;
    DynamicJsonDocument jsonMessage {1024};

};

#endif
