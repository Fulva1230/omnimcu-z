//
// Created by fulva on 11/22/19.
//

#ifndef OMNIMCU_Z_PDCONTROLLER_H
#define OMNIMCU_Z_PDCONTROLLER_H
#define PERIOD 100 //2 for 1ms

#include "car_variables.h"

namespace speedcon {
}
class PDcon {
public:
    void setInput(double input) {
        preInput = curInput;
        curInput = input;
    }

    double getOutput() {
        return Kp[0] * curInput + Kd[0] * (curInput - preInput) * 1000 / PERIOD;
    }

private:
    double preInput{};
    double curInput{};
};

#endif //OMNIMCU_Z_PDCONTROLLER_H
