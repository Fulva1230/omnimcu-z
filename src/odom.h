//
// Created by fulva on 12/6/19.
//

#ifndef OMNIMCU_Z_ODOM_H
#define OMNIMCU_Z_ODOM_H

#include <Twist.h>
#include <lib/odem.h>
#include <Odometry.h>
#include "Wheel.h"

namespace odom {
    struct {
        double x{0.0};
        double y{0.0};
        double ang{0.0};
    } odom;
    struct {
        ros::Time currentTime;
        ros::Time previousTIme;
    } timestamps;
    DeltaMove deltaMove;

    class OdomWheelWrapper {
    public:
        explicit OdomWheelWrapper(Wheel &wheel) : wheel(wheel) {}

        inline short getPreviousPosition() {
            return previousPosition;
        }

        inline void update() {
            previousPosition = wheel.motor.cPos;
        }

        inline Wheel &getWheel() {
            return wheel;
        }

    private:
        Wheel &wheel;
        short previousPosition{};
    };

    std::vector<OdomWheelWrapper> localUseWheels;
    ros::NodeHandle *nodeHandlePointer;
    nav_msgs::Odometry odem_message{};
    ros::Publisher odomPub("/odom", &odem_message);


    void wheelsinject(const std::vector<Wheel *> &wheels) {
        for (auto wheelp:wheels) {
            localUseWheels.emplace_back(*wheelp);
        }
    }

    void rosinject(ros::NodeHandle &nh) {
        nodeHandlePointer = &nh;
    }

    inline void fbGoalUpdate(const geometry_msgs::Twist &twist) {
        for (auto &odomwheelp: localUseWheels) {
            Wheel &wheel = odomwheelp.getWheel();
            wheel.motor.gSpeed =
                    (sin(wheel.theta) * twist.linear.x
                     - cos(wheel.theta) * twist.linear.y
                     - wheel.disToC * twist.angular.z) / wheel.radii;
        }
    }

    ros::Subscriber<geometry_msgs::Twist> vel_cmd("order", &fbGoalUpdate);

    void initialize() {
        nodeHandlePointer->advertise(odomPub);
        nodeHandlePointer->subscribe(vel_cmd);
    }

    inline DeltaWheels getDeltaWheelAndUpdate() {
        Wheel &wheel1 = localUseWheels[0].getWheel();
        double deltaWheel1An =
                overflowDiff(wheel1.motor.cPos, localUseWheels[0].getPreviousPosition()) * wheel1.motor.countToRadian;
        localUseWheels[0].update();
        Wheel &wheel2 = localUseWheels[1].getWheel();
        double deltaWheel2An =
                overflowDiff(wheel2.motor.cPos, localUseWheels[1].getPreviousPosition()) * wheel2.motor.countToRadian;
        localUseWheels[1].update();
        Wheel &wheel3 = localUseWheels[2].getWheel();
        double deltaWheel3An =
                overflowDiff(wheel3.motor.cPos, localUseWheels[2].getPreviousPosition()) * wheel3.motor.countToRadian;
        localUseWheels[2].update();
        Wheel &wheel4 = localUseWheels[3].getWheel();
        double deltaWheel4An =
                overflowDiff(wheel4.motor.cPos, localUseWheels[3].getPreviousPosition()) * wheel4.motor.countToRadian;
        localUseWheels[3].update();

        DeltaWheels deltaWheels{
                .wheel1An = deltaWheel1An,
                .wheel2An = deltaWheel2An,
                .wheel3An = deltaWheel3An,
                .wheel4An = deltaWheel4An
        };

        return deltaWheels;
    }

    inline void updateTimeStamp() {
        timestamps.previousTIme = timestamps.currentTime;
        timestamps.currentTime = nodeHandlePointer->now();
    }

    inline void publishOdom() {
        odem_message.header.stamp = timestamps.currentTime;
        odem_message.pose.pose.position.x = odom.x;
        odem_message.pose.pose.position.y = odom.y;
        odem_message.pose.pose.orientation.z = sin(odom.ang / 2);
        odem_message.pose.pose.orientation.w = cos(odom.ang / 2);
        double deltaT = timestamps.currentTime.toSec() - timestamps.previousTIme.toSec();
        odem_message.twist.twist.linear.x = deltaMove.deltax / deltaT;
        odem_message.twist.twist.linear.y = deltaMove.deltay / deltaT;
        odem_message.twist.twist.angular.z = deltaMove.deltaAngle / deltaT;
        odomPub.publish(&odem_message);
    }

    inline void updateOdom() {
        DeltaWheels deltaWheels = getDeltaWheelAndUpdate();
        WheelStats wheelStats{
                .wheel1roAn = localUseWheels[0].getWheel().theta,
                .wheel2roAn = localUseWheels[1].getWheel().theta,
                .wheel3roAn = localUseWheels[2].getWheel().theta,
                .wheel4roAn = localUseWheels[3].getWheel().theta
        };
        uint8_t disToC = localUseWheels[0].getWheel().disToC;
        uint8_t radii = localUseWheels[0].getWheel().radii;
        deltaMove = clcMove(deltaWheels, wheelStats, disToC, radii);
        odom.x += deltaMove.deltax;
        odom.y += deltaMove.deltay;
        odom.ang += deltaMove.deltaAngle;
    }
}

#endif //OMNIMCU_Z_ODOM_H
