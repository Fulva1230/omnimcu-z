//
// Created by fulva on 12/5/19.
//

#ifndef H_MMOTORSTATE
#define H_MMOTORSTATE

class MMotorState {
public:
    void step() {
        prePos = cPos;
    }

private:
    volatile double cSpeed{};
    volatile short &cPos;
    volatile short prePos{};
};

#endif

