#include "CountFunc.h"
#include <stm32f446xx.h>
#include <mbed.h>
#include <ros.h>
#include "debug.h"
#include "Initialization.h"

int main() {
    ros::NodeHandle nh;
    nh.initNode();
    initializer initializer{nh};
    initializer.initializeEveryThing();
    int counter = 0;
    while (true) {
        ++counter;
        if (counter == 20) {
            counter = 0;
            debug_n::debug();
            nh.getParam("/gains/Kp", Kp);
            nh.getParam("/gains/Kd", Kd);
        }
        odom::updateTimeStamp();
        odom::updateOdom();
        odom::publishOdom();
        nh.spinOnce();
        ThisThread::sleep_for(50);
    }
}
