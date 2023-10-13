#include "message.h"

message::message(const std::string type, std::string payload)
: type(type),
payload(payload)

{
}

void message::getMessage()
{
    jsonMessage["type"] = type;
    jsonMessage["payload"] = payload;

    serializeJson(jsonMessage, Serial);
    Serial.println();
}

