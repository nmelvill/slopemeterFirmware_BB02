#include "state.h"
#include <vector>

MotionState::MotionState(bool bigEndian, size_t byteNum)
: rawx(0), rawy(0), rawz(0), bigEndian(bigEndian), byteNum(byteNum)
{
    data.resize(6, 0);
}

void MotionState::update()
{

    getData();

    if (bigEndian) {
        rawx = data[0] <<8| data[1];     
        rawy = data[2] <<8| data[3];   
        rawz = data[4] <<8| data[5];
    }

    else {                                  //Little Endian
        rawx = data[1] <<8| data[0];     
        rawy = data[3] <<8| data[2];   
        rawz = data[5] <<8| data[4];
    }

    rawState = {rawx, rawy, rawz};
}

void MotionState::getData()
//Reads a byteNum numver of bytes off of the I2C buffer from the ICM 20948
{

    for (int i = 0; i < byteNum; i++)
    {
        data[i] = Wire.read();

    }

}

std::vector<int16_t> MotionState::getState()
{
    update();
    return rawState;
}

