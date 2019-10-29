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
    MMotor(volatile short &cPos, MotorConfig motorConfig, double countToRadian)
            : cPos(cPos), motorConfig{motorConfig}, countToRadian(countToRadian) {

    }

    volatile double cSpeed{};
    volatile short &cPos;
    volatile short prePos{};
    double gSpeed{};
    volatile bool update{};
    volatile double speedErrorIg{};
    MotorConfig motorConfig;
    double countToRadian;
};


#endif //OMNIMCU_Z_MMOTOR_H
