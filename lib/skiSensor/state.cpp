#include "state.h"

MotionState::MotionState()
: x(0), y(0), z(0)
{
    byte data[6];
}

void MotionState::update()
{

    //data[] = {(byte)Wire.read()};
    
    //x = data[0] <<8| data[1];     
    //y = data[2] <<8| data[3];   
    //z = data[4] <<8| data[5];  

    state = {x, y, z};
}

std::vector<int> MotionState::getState()
{
    update();
    return state;
}

