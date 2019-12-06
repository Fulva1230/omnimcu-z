//
// Created by fulva on 12/5/19.
//

#ifndef OMNIMCU_Z_INITIALIZATION_H
#define OMNIMCU_Z_INITIALIZATION_H

#include <ros.h>
#include "Wheel.h"
#include "debug.h"
#include "VehicleConfig.h"
#include "encoder.h"

class initializer {
public:
    initializer(ros::NodeHandle &nh) : nh(nh) {}

    void initializeEveryThing() {
        motorInit();
        modechange::initialize(nh);
        debug_n::rosnodeInject(nh);
        debug_n::initialize();
        vehicleconfig::initialize();
        speedonInit();
    }

private:
    void motorInit() {
        EncoderInitialiseTIM1();
        EncoderInitialiseTIM2();
        EncoderInitialiseTIM3();
        EncoderInitialiseTIM4();
    }

    ros::NodeHandle &nh;
};


#endif //OMNIMCU_Z_INITIALIZATION_H
