#include "message.h"
//Add class variable for switching between bluetooth and usb port

message::message(const std::string type, std::string payload, std::string status)
: type(type),
payload(payload),
status (status)

{
}


void message::buildMessage()
{
    jsonMessage["type"] = type;
    jsonMessage["payload"] = payload;
    jsonMessage["status"] = status;

    serializeJson(jsonMessage, Serial);
    Serial.println();
}


payload::payload(std::map<std::string, int> values)
: values(values)
{

}


void payload::buildJSON()
//Creates json object using ArduinoJson using a variable amount of values in an input map to the payload class
{

    for (auto i = values.begin(); i != values.end(); i++ ) {
        payloadValues[i->first] = i->second;
    }
}


std::string payload::build()
//Runs the buildJSON function to create the JSON object and then serializes it into a string
{
    buildJSON();
    serializeJson(payloadValues, jsonString);

    return jsonString;
}


messageRouter::messageRouter(const std::string routerType)
: routerType(routerType)
{
}


std::string messageRouter::handleInput(std::string input)
{
    //if (routerType == 'Serial')

        
}
