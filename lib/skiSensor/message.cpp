#include "message.h"
//Add class variable for switching between bluetooth and usb port

message::message(const std::string type, std::map<std::string, int> payloadValues, std::string status)
: type(type),
payloadValues(payloadValues),
status (status)
{
}


void message::buildMessage()
//Creates a json object then nests a payload json object inside then serializes all of it into a string and prints it
{
    jsonMessage["type"] = type;
    jsonMessage["status"] = status;
    buildPayload();

    serializeJson(jsonMessage, Serial);
    Serial.println();
}


void message::buildPayload()
//Creates nested json object using ArduinoJson using a variable amount of values in an input map to the payload class
{
    JsonObject payload = jsonMessage.createNestedObject("payload");

    for (auto i = payloadValues.begin(); i != payloadValues.end(); i++ ) {
        payload[i->first] = i->second;
    }
}


messageRouter::messageRouter(const std::string routerType)
: routerType(routerType)
{
}


std::string messageRouter::handleInput(std::string input)
{
    //if (routerType == 'Serial')
    std::string test = "test";
    return test;
        
}
