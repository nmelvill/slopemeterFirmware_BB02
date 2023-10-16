#include "message.h"
//Add class variable for switching between bluetooth and usb port

message::message(const std::string type, std::string payload, std::string status)
: type(type),
payload(payload),
status (status)

{
}

void message::getMessage()
{
    jsonMessage["type"] = type;
    jsonMessage["payload"] = payload;
    jsonMessage["status"] = status;

    serializeJson(jsonMessage, Serial);
    Serial.println();
}

