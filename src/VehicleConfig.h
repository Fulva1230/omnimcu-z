//
// Created by fulva on 10/28/19.
//

#ifndef OMNIMCU_Z_VEHICLECONFIG_H
#define OMNIMCU_Z_VEHICLECONFIG_H

#include "MMotor.h"
#include "Wheel.h"

#define DISTANCE_TO_CENTER 80
#define RADIUS_OF_WHEEL 40
#define COUNT_TO_RADIAN 0.02243994753

//The order of motors and wheels is from right-up most with counter clock wise.

MMotor motor1{(short &) TIM1->CNT,
              MotorConfig{
                      .pin1=DigitalOut{PC_0},
                      .pin2=DigitalOut{PC_3},
                      .pinena=PwmOut{PF_7}
              }, COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel1{
        .motor = motor1,
        .theta = M_PI * 1 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};

MMotor motor2{(short &) TIM2->CNT,
              MotorConfig{
                      .pin1=DigitalOut{PF_3},
                      .pin2=DigitalOut{PF_5},
                      .pinena=PwmOut{PF_8}
              }, COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel2{
        .motor = motor2,
        .theta = M_PI * 3 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};


MMotor motor3{(short &) TIM3->CNT,
              MotorConfig{
                      .pin1=DigitalOut{PF_1},
                      .pin2=DigitalOut{PF_2},
                      .pinena=PwmOut{PE_5}
              }, COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel3{
        .motor = motor3,
        .theta = M_PI * 5 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};


MMotor motor4{(short &) TIM4->CNT,
              MotorConfig{
                      .pin1=DigitalOut{PD_0},
                      .pin2=DigitalOut{PD_1},
                      .pinena=PwmOut{PE_6}
              }, COUNT_TO_RADIAN}; // for 140 count :1 radian

Wheel wheel4{
        .motor = motor4,
        .theta = M_PI * 7 / 4,
        .radii = RADIUS_OF_WHEEL,
        .disToC = DISTANCE_TO_CENTER,
};


#endif //OMNIMCU_Z_VEHICLECONFIG_H
