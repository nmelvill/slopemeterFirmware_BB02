#ifndef PROPERTY_H
#define PROPERTY_H

#include "Arduino.h"
#include <vector>
#include "Wire.h"

class MotionState
{
public:
    MotionState(bool bigEndian, int byteNum);
    std::vector<int> getState();

private:
    int16_t x;
    int16_t y;
    int16_t z;
    int8_t status;
    bool bigEndian;
    std::vector<int> data;
    std::vector<int> state = {x, y, z};
    void getData();
    int byteNum;


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