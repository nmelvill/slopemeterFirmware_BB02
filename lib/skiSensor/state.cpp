#include "state.h"
#include <vector>

MotionState::MotionState(bool bigEndian, size_t byteNum)
: x(0), y(0), z(0), bigEndian(bigEndian), byteNum(byteNum)
{
    data.resize(6, 0);
}

void MotionState::update()
{

    getData();

    if (bigEndian) {
        x = data[0] <<8| data[1];     
        y = data[2] <<8| data[3];   
        z = data[4] <<8| data[5];
    }

    else {                                  //Little Endian
        x = data[1] <<8| data[0];     
        y = data[3] <<8| data[2];   
        z = data[5] <<8| data[4];
    }

    state = {x, y, z};
}

void MotionState::getData()
//Reads a byteNum numver of bytes off of the I2C buffer from the ICM 20948
{

    for (int i = 0; i < byteNum; i++)
    {
        data[i] = Wire.read();
        //Serial.println(data[i],BIN);

    }

}

std::vector<int> MotionState::getState()
{
    update();
    return state;
}

