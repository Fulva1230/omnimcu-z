//
// Created by Fulva on 11/17/2019.
//

#ifndef OMNIMCU_Z_ODEM_H
#define OMNIMCU_Z_ODEM_H

#include <cmath>

struct DeltaMove {
    double deltax;
    double deltay;
    double deltaAngle;
};
struct DeltaWheels {
    const double wheel1An;
    const double wheel2An;
    const double wheel3An;
    const double wheel4An;
};
struct WheelStats {
    const double wheel1roAn;
    const double wheel2roAn;
    const double wheel3roAn;
    const double wheel4roAn;
};

struct Cramer {
    double delta;
    double deltaX;
    double deltaY;

    double x() {
        return deltaX / delta;
    }

    double y() {
        return deltaY / delta;
    }
};

inline Cramer
getDisplacement(const uint8_t disToC, const uint8_t wheelRadii, const double wheel2roAn, const double wheel3roAn,
                double deltaMotor2An,
                double deltaMotor3An,
                double deltaAng) {
    const double sinWh2 = sin(wheel2roAn);
    const double cosWh3 = cos(wheel3roAn);
    const double cosWh2 = cos(wheel2roAn);
    const double sinWh3 = sin(wheel3roAn);
    return Cramer{
            .delta = -sinWh2 * cosWh3 + cosWh2 * sinWh3,
            .deltaX = (deltaMotor2An * wheelRadii + deltaAng * disToC) * (-cosWh3) -
                      (-cosWh2) * (deltaMotor3An * wheelRadii + deltaAng * disToC),
            .deltaY = sinWh2 * (deltaMotor3An * wheelRadii + deltaAng * disToC) -
                      (deltaMotor2An * wheelRadii + deltaAng * disToC) * sinWh3
    };
}

const DeltaMove
clcMove(const DeltaWheels deltaWheels, const WheelStats wheelStats, const uint8_t disToC, const uint8_t wheelRadii) {
    double deltaAng =
            -(deltaWheels.wheel1An + deltaWheels.wheel2An + deltaWheels.wheel3An + deltaWheels.wheel4An) * wheelRadii /
            (4 * disToC);

    Cramer s23 = getDisplacement(disToC, wheelRadii, wheelStats.wheel2roAn, wheelStats.wheel3roAn, deltaWheels.wheel2An,
                                 deltaWheels.wheel3An, deltaAng);
    Cramer s41 = getDisplacement(disToC, wheelRadii, wheelStats.wheel4roAn, wheelStats.wheel1roAn, deltaWheels.wheel4An,
                                 deltaWheels.wheel1An, deltaAng);

    double deltaX = (s23.x() + s41.x()) / 2;
    double deltaY = (s23.y() + s41.y()) / 2;

    return DeltaMove{
            .deltax = deltaX,
            .deltay = deltaY,
            .deltaAngle = deltaAng
    };
}

inline short overflowDiff(short cPos, short prePos) {
    int diff(cPos - prePos);
    if (diff > INT16_MAX) {
        return -(INT32_MAX - diff + 1);
    } else if (diff < INT16_MIN) {
        return -(INT32_MIN - diff);
    }
    return diff;
}

#endif //OMNIMCU_Z_ODEM_H
