#ifndef _INPUT_H
#define _INPUT_H

#include "Wire.h"
#include "MPU9250.h"
#include "Tetris.h"

extern MPU9250 Sensor;
void init_mpu();

class Input {
public:
    Input();
    void measure();
    int shift();
    bool rotate();
private:
    bool last_was_rot;
};

#endif //_INPUT
