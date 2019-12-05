//
// Created by fulva on 10/25/19.
//

#include <mbed.h>
#include "PDController.h"

#ifndef OMNIMCU_Z_MMOTOR_H
#define OMNIMCU_Z_MMOTOR_H

struct MotorConfig {
    DigitalOut pin1;
    DigitalOut pin2;
    PwmOut pinena;

    void init() {
        pinena.period_ms(20);
    }
};


class MMotor {
public:
    MMotor(volatile short &cPos, MotorConfig &motorConfig, double countToRadian)
            : cPos(cPos), motorConfig{motorConfig}, countToRadian(countToRadian) {
    }

    void drive(double speed) {
        if (speed < 0) {
            motorConfig.pin1.write(0);
            motorConfig.pin2.write(1);
        } else {
            motorConfig.pin1.write(1);
            motorConfig.pin2.write(0);
        }

        motorConfig.pinena.write(abs(speed));
    }

    volatile double cSpeed{};
    volatile short &cPos;
    volatile short prePos{};
    double gSpeed{};
    volatile double speedErrorIg{};
    MotorConfig &motorConfig;
    double countToRadian;
    PDcon pDcon;
};


#endif //OMNIMCU_Z_MMOTOR_H
