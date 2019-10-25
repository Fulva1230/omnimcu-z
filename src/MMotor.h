//
// Created by fulva on 10/25/19.
//

#include <mbed.h>

#ifndef OMNIMCU_Z_MMOTOR_H
#define OMNIMCU_Z_MMOTOR_H

struct MotorConfig {
    DigitalOut pin1;
    DigitalOut pin2;
    PwmOut pinena;
};


class MMotor {
public:
    MMotor(volatile uint32_t &cPos, MotorConfig motorConfig) : cPos(cPos), motorConfig{motorConfig} {

    }

    volatile int cSpeed{};
    volatile uint32_t &cPos;
    volatile short prePos{};
    int gSpeed{};
    volatile bool update{};
    volatile int speedErrorIg{};
    MotorConfig motorConfig;
};


#endif //OMNIMCU_Z_MMOTOR_H
