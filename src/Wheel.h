//
// Created by fulva on 10/28/19.
//

#ifndef OMNIMCU_Z_WHEEL_H
#define OMNIMCU_Z_WHEEL_H

#include <cstdint>
#include "MMotor.h"

struct Wheel {
    MMotor &motor;
    double theta;// the current orientation
    uint8_t radii;// unit mm
    uint8_t disToC; //unit mm
};

#endif //OMNIMCU_Z_WHEEL_H
