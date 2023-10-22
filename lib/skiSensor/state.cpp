#include "state.h"

MotionState::MotionState(bool bigEndian, int byteNum)
: x(0), y(0), z(0), bigEndian(bigEndian), byteNum(byteNum)
{
}

void MotionState::update()
{

    getData();
    
    x = data[0] <<8| data[1];     
    y = data[2] <<8| data[3];   
    z = data[4] <<8| data[5];  

    state = {x, y, z};
}

void MotionState::getData()
//Reads a byteNum numver of bytes off of the I2C buffer from the ICM 20948
{
    for (int i = 0; i < byteNum; i++)
    {
        data[i] = Wire.read();

    }

}

std::vector<int> MotionState::getState()
{
    update();
    return state;
}

