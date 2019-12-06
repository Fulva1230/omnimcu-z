#include "CountFunc.h"
#include <stm32f446xx.h>
#include <mbed.h>
#include <ros.h>
#include "debug.h"
#include "Initialization.h"


void updateOdem(ros::NodeHandle &nh);


int main() {
    ros::NodeHandle nh;
    nh.initNode();
    initializer initializer{nh};
    initializer.initializeEveryThing();
    CountFunc intervalexec{[&]() {
        debuging(nh, g_wheels, 4);
        nh.getParam("/gains/Kp", Kp);
        nh.getParam("/gains/Kd", Kd);
    }, 20};
    while (true) {
        intervalexec();
        odom::updateOdom();
        odom::publishOdom();
        nh.spinOnce();
        ThisThread::sleep_for(50);
    }
}
