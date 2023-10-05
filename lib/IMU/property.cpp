#include "property.h"

MotionState::MotionState()
: x(0), y(0), z(0)
{

}

void MotionState::update()
{
    x = Wire.read() <<8| Wire.read();     
    y = Wire.read() <<8| Wire.read();  
    z = Wire.read() <<8| Wire.read();  

    state = {x, y, z};
}

std::vector<int> MotionState::getState()
{
    update();
    return state;
}
