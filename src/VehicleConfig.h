//
// Created by fulva on 10/28/19.
//

#ifndef OMNIMCU_Z_VEHICLECONFIG_H
#define OMNIMCU_Z_VEHICLECONFIG_H

#include "MMotor.h"
#include "Wheel.h"

#define DISTANCE_TO_CENTER 139
#define RADIUS_OF_WHEEL 50
//TODO this value should change to fit the new circumstance
#define COUNT_TO_RADIAN 0.0044879895051282755

//The order of motors and wheels is from right-up most with counter clock wise.
MotorConfig motor1Config{
        .pin1 = DigitalOut{PC_0},
        .pin2 = DigitalOut{PC_3},
        .pinena = PE_5
};
MMotor motor1{(short &) TIM1->CNT, motor1Config,
              COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel1{
        .motor = motor1,
        .theta = M_PI * 1 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};

MotorConfig motor2Config{
        .pin1=DigitalOut{PF_3},
        .pin2=DigitalOut{PF_5},
        .pinena=PF_8
};
MMotor
        motor2{
        (short &) TIM2->CNT, motor2Config,
        COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel2{
        .motor = motor2,
        .theta = M_PI * 3 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};

MotorConfig motor3Config{
        .pin1=DigitalOut{PF_1},
        .pin2=DigitalOut{PF_2},
        .pinena=PF_9
};

MMotor motor3{(short &) TIM3->CNT, motor3Config, COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel3{
        .motor = motor3,
        .theta = M_PI * 5 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};


MotorConfig motor4Config{
        .pin1=DigitalOut{PD_0},
        .pin2=DigitalOut{PD_1},
        .pinena=PE_6
};
MMotor motor4{(short &) TIM4->CNT, motor4Config, COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel4{
        .motor = motor4,
        .theta = M_PI * 7 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};


#endif //OMNIMCU_Z_VEHICLECONFIG_H
