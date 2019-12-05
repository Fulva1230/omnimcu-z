//
// Created by fulva on 12/5/19.
//

#ifndef OMNIMCU_Z_INITIALIZATION_H
#define OMNIMCU_Z_INITIALIZATION_H

#include <ros.h>
#include "Wheel.h"
#include "debug.h"

class initializer {
public:
    void initializeEveryThing() {
        modechange::initialize(nh);
        debug_n::initialize(nh);
    }

private:
    ros::NodeHandle &nh;
};


#endif //OMNIMCU_Z_INITIALIZATION_H
