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
    void send();

    static char s_routerType;
    

    private:
    std::string type;
    std::string payloadString;
    std::string status;
    DynamicJsonDocument jsonMessage {1024};
    std::map<std::string, int> payloadValues;
    
    void buildPayload();

};


#endif
