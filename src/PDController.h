//
// Created by fulva on 11/22/19.
//

#ifndef OMNIMCU_Z_PDCONTROLLER_H
#define OMNIMCU_Z_PDCONTROLLER_H
#define PERIOD 100 //2 for 1ms

namespace speedcon {
    extern float Kp[1]{1};
    extern float Kd[1]{0};


}
class PDcon {
public:
    void setInput(double input) {
        preInput = curInput;
        curInput = input;
    }

    double getOutput() {
        return speedcon::Kp[0] * curInput + speedcon::Kd[0] * (curInput - preInput) * 1000 / PERIOD;
    }

private:
    double preInput{};
    double curInput{};
};

#endif //OMNIMCU_Z_PDCONTROLLER_H
