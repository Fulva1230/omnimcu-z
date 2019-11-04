#include <VehicleConfig.h>
#include <src/speedcon.h>
#include <src/encoder.h>
#include <stm32f446xx.h>
#include <cstdio>
#include <mbed.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <time.h>
#include <tf/transform_broadcaster.h>

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
std_msgs::String debug_message;
ros::Publisher debugros("mcudebug", &debug_message);

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

int main() {
    motorInit();
    ros::NodeHandle nh;
    nh.initNode();
    myled = nh.subscribe(sub);
    nh.advertise(debugros);
    broadcaster.init(nh);
    timer.start();

    while (true) {
        reference_wrapper<Wheel> wheels[] = {wheel1, wheel2, wheel3, wheel4};
        debug_message.data = "";
        for (Wheel &wheel: wheels) {
            debug_message.data = std::to_string(wheel.motor.gSpeed).c_str();
            debugros.publish(&debug_message);
        }
        updateOdem(nh);
        nh.spinOnce();
        ThisThread::sleep_for(50);
    }
}

struct Cramer {
    double delta;
    double deltaX;
    double deltaY;

    double x() {
        return deltaX / delta;
    }

    double y() {
        return deltaY / delta;
    }
};

inline Cramer getDisplacement(const Wheel &wheel2, const Wheel &wheel3, double deltaMotor2An, double deltaMotor3An,
                              double deltaAng) {
    const double sinWh2 = sin(wheel2.theta);
    const double cosWh3 = cos(wheel3.theta);
    const double cosWh2 = cos(wheel2.theta);
    const double sinWh3 = sin(wheel3.theta);
    return Cramer{
            .delta = -sinWh2 * cosWh3 + cosWh2 * sinWh3,
            .deltaX = (deltaMotor2An * wheel2.radii + deltaAng * wheel2.disToC) * (-cosWh3) -
                    (-cosWh2) * (deltaMotor3An * wheel3.radii + deltaAng * wheel3.disToC),
            .deltaY = sinWh2 * (deltaMotor3An * wheel3.radii + deltaAng * wheel3.disToC) -
                    (deltaMotor2An * wheel2.radii + deltaAng * wheel2.disToC) * sinWh3
    };
}

void updateOdem(ros::NodeHandle &nh) {
    static double x{};
    static double y{};
    static double ang{};
    static short wheel1prepos{};
    static short wheel2prepos{};
    static short wheel3prepos{};
    static short wheel4prepos{};

    static int preTime = 0;
    int curTime = timer.read_ms();
    double delta_time = 0.001 * (curTime - preTime);

    double deltaWheel1An = (wheel1.motor.cPos - wheel1prepos) * wheel1.motor.countToRadian;
    wheel1prepos = wheel1.motor.cPos;
    double deltaWheel2An = (wheel2.motor.cPos - wheel2prepos) * wheel2.motor.countToRadian;
    wheel2prepos = wheel2.motor.cPos;
    double deltaWheel3An = (wheel3.motor.cPos - wheel3prepos) * wheel3.motor.countToRadian;
    wheel3prepos = wheel3.motor.cPos;
    double deltaWheel4An = (wheel4.motor.cPos - wheel4prepos) * wheel4.motor.countToRadian;
    wheel4prepos = wheel4.motor.cPos;

    double deltaAng =
            -(deltaWheel1An + deltaWheel2An + deltaWheel3An + deltaWheel4An) * wheel1.radii / (4 * wheel1.disToC);
    ang += deltaAng;
    wheel1.theta += deltaAng;
    wheel2.theta += deltaAng;
    wheel3.theta += deltaAng;
    wheel4.theta += deltaAng;

    Cramer s23 = getDisplacement(wheel2, wheel3, deltaWheel2An, deltaWheel3An, deltaAng);
    Cramer s41 = getDisplacement(wheel4, wheel1, deltaWheel4An, deltaWheel1An, deltaAng);

    double deltaX = (s23.x() + s41.x()) / 2;
    double deltaY = (s23.y() + s41.y()) / 2;
    x += deltaX;
    y += deltaY;

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


    preTime = curTime;
}
