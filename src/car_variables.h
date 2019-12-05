//
// Created by fulva on 11/29/19.
//

#ifndef OMNIMCU_Z_CAR_VARIABLES_H
#define OMNIMCU_Z_CAR_VARIABLES_H

#include <subscriber.h>
#include <std_msgs/String.h>
#include <functional>
#include <vector>
#include <ros.h>

double x{};
double y{};
double ang{};
std::vector<std::function<void(std::string)>> modesubscribers;

extern float Kp[1]{1};
extern float Kd[1]{0};

void mode_change_cb(const std_msgs::String &mode) {
    std::string mode_str = mode.data;
    for (const auto &func:modesubscribers) {
        func(mode_str);
    }
}

namespace modechange {
    ros::Subscriber<std_msgs::String> mode_change{"mode", mode_change_cb};

    void initialize(ros::NodeHandle &nh) {
        nh.subscribe(mode_change);
    }
}

#endif //OMNIMCU_Z_CAR_VARIABLES_H
