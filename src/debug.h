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
    Wheel *localUseWheels;

    void wheelsinject(Wheel *wheels) {
        localUseWheels = wheels;
    }

    void rosnodeInject(ros::NodeHandle &nh) {
        nodeHandleLocal = &nh;
    }

    void initialize() {
        nodeHandleLocal->advertise(debugros);
    }

    inline string wheelMessage(Wheel &wheel) {
        string message{};
        message.append("gSpeed:");
        message.append(std::to_string(wheel.motor.gSpeed));
        message.append("  count:");
        message.append(std::to_string(wheel.motor.cPos));
        message.append(" cSpeed:");
        message.append(std::to_string(wheel.motor.cSpeed));
        return message;
    }

    inline std_msgs::Header debugMsgGenerate(int whichMotor) {
        std_msgs::Header the_debug_message{};
        the_debug_message.frame_id = "";
        std::string message{};
        message.append(std::to_string(whichMotor + 1));
        message.append("::");
        message.append(wheelMessage(localUseWheels[whichMotor]));
        message.append("::");
        the_debug_message.frame_id = message.c_str();
        the_debug_message.stamp = nodeHandleLocal->now();
        return the_debug_message;
    }

    inline void debug() {
        for (int i = 0; i < 4; ++i) {
            debug_message = debugMsgGenerate(i);
            debugros.publish(&debug_message);
        }
    }
};

#endif //OMNIMCU_Z_DEBUG_H
