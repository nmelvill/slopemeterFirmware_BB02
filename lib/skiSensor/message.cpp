#include "message.h"

message::message(const std::string type, std::map<std::string, int> payloadValues, std::string status)
: type(type),
payloadValues(payloadValues),
status (status)
{
}

char message::s_routerType;


void message::buildMessage()
//Creates a json object then nests a payload json object inside then serializes all of it into a string and prints it
{
    jsonMessage["type"] = type;
    jsonMessage["status"] = status;
    buildPayload();
    
}


void message::buildPayload()
//Creates nested json object using ArduinoJson using a variable amount of values in an input map to the payload class
{
    JsonObject payload = jsonMessage.createNestedObject("payload");

    for (auto i = payloadValues.begin(); i != payloadValues.end(); i++ ) {
        payload[i->first] = i->second;
    }
}



void message::send()
{
    
    switch(s_routerType)
    {
    case 0:
        serializeJson(jsonMessage, Serial); Serial.println();
        break;
    
    case 1:
        break;
    
    case 2:
        serializeJson(jsonMessage, Serial); Serial.println();
        break;
    }
}
