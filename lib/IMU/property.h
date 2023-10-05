#ifndef PROPERTY_H
#define PROPERTY_H

#include "Arduino.h"
#include <vector>
#include "Wire.h"

class MotionState
{
public:
    MotionState();
    std::vector<int> getState();

private:
    int16_t x;
    int16_t y;
    int16_t z;
    std::vector<int> state = {x, y, z};

    void update();
};


class scalar
{
public:
    scalar();
    uint16_t getValues();

private:
    uint16_t value = 0;
};



#endif