//
// Created by fulva on 12/2/19.
//

#ifndef OMNIMCU_Z_DEBUG_H
#define OMNIMCU_Z_DEBUG_H

#include <std_msgs/Header.h>


string wheelMessage(Wheel &wheel) {
    string message{};
    message.append("gSpeed:");
    message.append(std::to_string(wheel.motor.gSpeed));
    message.append("  count:");
    message.append(std::to_string(wheel.motor.cPos));
    message.append(" cSpeed:");
    message.append(std::to_string(wheel.motor.cSpeed));
    return message;
}

std_msgs::Header debuging(ros::NodeHandle &nh, reference_wrapper<Wheel> *wheels, int len) {
    std_msgs::Header the_debug_message{};
    the_debug_message.frame_id = "";
    for (int i = 0; i < len; ++i) {
        string message{};
        message.append(std::to_string(i + 1));
        message.append("::");
        message.append(wheelMessage(wheels[i]));
        the_debug_message.frame_id = message.c_str();
        the_debug_message.stamp = nh.now();
        return the_debug_message;
    }
}

#endif //OMNIMCU_Z_DEBUG_H
