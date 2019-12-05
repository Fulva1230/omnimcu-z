#include "car_variables.h"
#include "CountFunc.h"
#include <VehicleConfig.h>
#include <src/speedcon.h>
#include <src/encoder.h>
#include <stm32f446xx.h>
#include <mbed.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Header.h>
#include <tf/transform_broadcaster.h>
#include <Odometry.h>
#include <odem.h>
#include <std_msgs/String.h>
#include "debug.h"

DigitalOut myled(LED1);

ros::Subscriber<geometry_msgs::Twist> vel_cmd("order", &fbGoalUpdate);
std_msgs::Header debug_message;
ros::Publisher debugros("mcudebug", &debug_message);
nav_msgs::Odometry odem_message{};
ros::Publisher odemPub("/odom", &odem_message);

tf::TransformBroadcaster tfbroadcaster;

void motorInit() {
    speedcon::motors[0] = &motor1;
    speedcon::motors[1] = &motor2;
    speedcon::motors[2] = &motor3;
    speedcon::motors[3] = &motor4;
    speedonInit();
    feedbackconInit();
    EncoderInitialiseTIM1();
    EncoderInitialiseTIM2();
    EncoderInitialiseTIM3();
    EncoderInitialiseTIM4();
}

void updateOdem(ros::NodeHandle &nh);


int main() {
    motorInit();
    ros::NodeHandle nh;
    nh.initNode();
    myled = nh.subscribe(vel_cmd);
    nh.subscribe(modechange::mode_change);
    nh.advertise(debugros);
    nh.advertise(odemPub);
    tfbroadcaster.init(nh);
    CountFunc intervalexec{[&]() {
        debuging(nh, g_wheels, 4);
        nh.getParam("/gains/Kp", Kp);
        nh.getParam("/gains/Kd", Kd);
    }, 20};
    while (true) {
        updateOdem(nh);
        intervalexec();
        nh.spinOnce();
        ThisThread::sleep_for(50);
    }
}

void updateOdem(ros::NodeHandle &nh) {
    static ros::Time preTime{nh.now() -= ros::Duration{1, 0}};
    static ros::Time curTime{nh.now()};
    static short wheel1prepos{};
    static short wheel2prepos{};
    static short wheel3prepos{};
    static short wheel4prepos{};

    curTime = nh.now();
    double deltaWheel1An = overflowDiff(wheel1.motor.cPos, wheel1prepos) * wheel1.motor.countToRadian;
    wheel1prepos = wheel1.motor.cPos;
    double deltaWheel2An = overflowDiff(wheel2.motor.cPos, wheel2prepos) * wheel2.motor.countToRadian;
    wheel2prepos = wheel2.motor.cPos;
    double deltaWheel3An = overflowDiff(wheel3.motor.cPos, wheel3prepos) * wheel3.motor.countToRadian;
    wheel3prepos = wheel3.motor.cPos;
    double deltaWheel4An = overflowDiff(wheel4.motor.cPos, wheel4prepos) * wheel4.motor.countToRadian;
    wheel4prepos = wheel4.motor.cPos;

    DeltaWheels deltaWheels{
            .wheel1An = deltaWheel1An,
            .wheel2An = deltaWheel2An,
            .wheel3An = deltaWheel3An,
            .wheel4An = deltaWheel4An
    };

    WheelStats wheelStats{
            .wheel1roAn = wheel1.theta,
            .wheel2roAn = wheel2.theta,
            .wheel3roAn = wheel3.theta,
            .wheel4roAn = wheel4.theta
    };

    auto deltaMove = clcMove(deltaWheels, wheelStats, wheel1.disToC, wheel1.radii);

    ang += deltaMove.deltaAngle;
    wheel1.theta += deltaMove.deltaAngle;
    wheel2.theta += deltaMove.deltaAngle;
    wheel3.theta += deltaMove.deltaAngle;
    wheel4.theta += deltaMove.deltaAngle;
    x += deltaMove.deltax;
    y += deltaMove.deltay;

    odem_message.header.stamp = curTime;
    odem_message.pose.pose.position.x = x;
    odem_message.pose.pose.position.y = y;
    odem_message.pose.pose.orientation.z = sin(ang / 2);
    odem_message.pose.pose.orientation.w = cos(ang / 2);
    double deltaT = curTime.toSec() - preTime.toSec();
    odem_message.twist.twist.linear.x = deltaMove.deltax / deltaT;
    odem_message.twist.twist.linear.y = deltaMove.deltay / deltaT;
    odem_message.twist.twist.angular.z = deltaMove.deltaAngle / deltaT;
    odemPub.publish(&odem_message);


//    geometry_msgs::TransformStamped t{};
//    t.header.frame_id = odom;
//    t.child_frame_id = base_link;
//    t.transform.translation.x = x;
//    t.transform.translation.y = y;
//    t.transform.translation.z = 0;
//    t.transform.rotation.x = 0.0;
//    t.transform.rotation.y = 0.0;
//    t.transform.rotation.z = sin(ang / 2);
//    t.transform.rotation.w = cos(ang / 2);
//    t.header.stamp = nh.now();
//    tfbroadcaster.sendTransform(t);

    preTime = curTime;
}
