#ifndef PROPERTY_H
#define PROPERTY_H

#include "Arduino.h"
#include <vector>
#include "Wire.h"

class MotionState
{
public:
    MotionState(bool bigEndian, size_t byteNum);
    std::vector<int> getState();

private:
    int16_t rawx;
    int16_t rawy;
    int16_t rawz;
    int8_t status;
    size_t byteNum;
    bool bigEndian;
    std::vector<uint8_t> data;
    std::vector<int> rawState = {rawx, rawy, rawz};
    void getData();
    


    void update();
};


class State
{
public:
    State();
    uint16_t getValues();

private:
    uint16_t value = 0;
};



#endif