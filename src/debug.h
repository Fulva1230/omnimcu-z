//
// Created by fulva on 12/2/19.
//

#ifndef OMNIMCU_Z_DEBUG_H
#define OMNIMCU_Z_DEBUG_H

#include <std_msgs/Header.h>
#include "Wheel.h"


namespace debug_n {
    std_msgs::Header debug_message;
    ros::Publisher debugros("mcudebug", &debug_message);
    ros::NodeHandle *nodeHandleLocal;
    vector <std::reference_wrapper<Wheel>> localUseWheels;

    void wheelsinject(const std::vector<Wheel *> &wheels) {
        for (auto wheelp:wheels) {
            localUseWheels.emplace_back(*wheelp);
        }
    }

    void rosnodeInject(ros::NodeHandle &nh) {
        nodeHandleLocal = &nh;
    }

    void initialize() {
        nodeHandleLocal->advertise(debugros);
    }

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

    std_msgs::Header debugMsgGenerate() {
        std_msgs::Header the_debug_message{};
        the_debug_message.frame_id = "";
        std::string message{};
        for (int i = 0; i < localUseWheels.size(); ++i) {
            message.append(std::to_string(i + 1));
            message.append("::");
            message.append(wheelMessage(localUseWheels[i]));
            message.append("::");
        }
        the_debug_message.frame_id = message.c_str();
        the_debug_message.stamp = nodeHandleLocal->now();
        return the_debug_message;
    }

    void debug() {
        debug_message = debugMsgGenerate();
        debugros.publish(&debug_message);
    }
};

#endif //OMNIMCU_Z_DEBUG_H
