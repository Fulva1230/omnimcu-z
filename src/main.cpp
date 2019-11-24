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

DigitalOut myled(LED1);

void goalUpdate(const geometry_msgs::Twist &twist) {
    reference_wrapper<Wheel> wheels[] = {wheel1, wheel2, wheel3, wheel4};
    for (Wheel &wheel: wheels) {
        wheel.motor.gSpeed =
                (sin(wheel.theta) * twist.linear.x
                 - cos(wheel.theta) * twist.linear.y
                 - wheel.disToC * twist.angular.z) / wheel.radii;
    }
}

ros::Subscriber<geometry_msgs::Twist> sub("order", &goalUpdate);
std_msgs::Header debug_message;
ros::Publisher debugros("mcudebug", &debug_message);
nav_msgs::Odometry odem_message{};
ros::Publisher odemPub("/odem", &odem_message);
double x{};
double y{};
double ang{};

tf::TransformBroadcaster broadcaster;
const char base_link[] = "/base_link";
const char odom[] = "/odom";

void motorInit() {
    speedcon::motors[0] = &motor1;
    speedcon::motors[1] = &motor2;
    speedcon::motors[2] = &motor3;
    speedcon::motors[3] = &motor4;
    sppedconInit();
    EncoderInitialiseTIM1();
    EncoderInitialiseTIM2();
    EncoderInitialiseTIM3();
    EncoderInitialiseTIM4();
}

Timer timer;

void updateOdem(ros::NodeHandle &nh);

void debuging(ros::NodeHandle &nh) {
    reference_wrapper<Wheel> wheels[] = {wheel1, wheel2, wheel3, wheel4};
    debug_message.frame_id = "";
    for (int i = 0; i < 4; ++i) {
        string message{};
        message.append(std::to_string(i + 1));
        message.append("::");
        message.append("gSpeed:");
        message.append(std::to_string(wheels[i].get().motor.gSpeed));
        message.append("  count:");
        message.append(std::to_string(wheels[i].get().motor.cPos));
        message.append(" cSpeed:");
        message.append(std::to_string(wheels[i].get().motor.cSpeed));
        debug_message.frame_id = message.c_str();
        debug_message.stamp = nh.now();
        debugros.publish(&debug_message);
    }
}


int main() {
    motorInit();
    ros::NodeHandle nh;
    nh.initNode();
    myled = nh.subscribe(sub);
    nh.advertise(debugros);
    nh.advertise(odemPub);
    broadcaster.init(nh);
    timer.start();
    unsigned long counter{0};
    while (true) {
        updateOdem(nh);
        if (counter % 20 == 0) {
            debuging(nh);
            nh.getParam("/gains/Kp", speedcon::Kp);
            nh.getParam("/gains/Kd", speedcon::Kd);
            counter = 0;
        }
        nh.spinOnce();
        ThisThread::sleep_for(50);
        ++counter;
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

    double deltaWheel1An = (wheel1.motor.cPos - wheel1prepos) * wheel1.motor.countToRadian;
    wheel1prepos = wheel1.motor.cPos;
    double deltaWheel2An = (wheel2.motor.cPos - wheel2prepos) * wheel2.motor.countToRadian;
    wheel2prepos = wheel2.motor.cPos;
    double deltaWheel3An = (wheel3.motor.cPos - wheel3prepos) * wheel3.motor.countToRadian;
    wheel3prepos = wheel3.motor.cPos;
    double deltaWheel4An = (wheel4.motor.cPos - wheel4prepos) * wheel4.motor.countToRadian;
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
    odem_message.pose.pose.orientation.z = sin(ang) / 2;
    odem_message.pose.pose.orientation.w = cos(ang);
    double deltaT = curTime.toSec() - preTime.toSec();
    odem_message.twist.twist.linear.x = deltaMove.deltax / deltaT;
    odem_message.twist.twist.linear.y = deltaMove.deltay / deltaT;
    odem_message.twist.twist.angular.z = deltaMove.deltaAngle / deltaT;
    odemPub.publish(&odem_message);


    geometry_msgs::TransformStamped t{};
    t.header.frame_id = odom;
    t.child_frame_id = base_link;
    t.transform.translation.x = x;
    t.transform.translation.y = y;
    t.transform.translation.z = 0;
    t.transform.rotation.x = 0.0;
    t.transform.rotation.y = 0.0;
    t.transform.rotation.z = sin(ang) / 2;
    t.transform.rotation.w = cos(ang);
    t.header.stamp = nh.now();
    broadcaster.sendTransform(t);

    curTime = preTime;
}
