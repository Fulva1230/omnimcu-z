//
// Created by fulva on 11/24/19.
//

#include <gtest/gtest.h>
#include "odem.h"


TEST(testodem, first) {
    WheelStats wheelStats{
            .wheel1roAn = M_PI / 4,
            .wheel2roAn = M_PI * 3 / 4,
            .wheel3roAn = M_PI * 5 / 4,
            .wheel4roAn = M_PI * 7 / 4
    };

    DeltaWheels deltaWheels{
            .wheel1An = -0.1,
            .wheel2An = 0.1,
            .wheel3An = 0.1,
            .wheel4An = -0.1
    };

    DeltaMove deltaMove = clcMove(deltaWheels, wheelStats, 100, 10);
    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltaAngle, 0.0);
}

TEST(testodem, second) {
    WheelStats wheelStats{
            .wheel1roAn = M_PI / 4,
            .wheel2roAn = M_PI * 3 / 4,
            .wheel3roAn = M_PI * 5 / 4,
            .wheel4roAn = M_PI * 7 / 4
    };

    DeltaWheels deltaWheels{
            .wheel1An = 0.1,
            .wheel2An = -0.1,
            .wheel3An = -0.1,
            .wheel4An = 0.1
    };

    DeltaMove deltaMove = clcMove(deltaWheels, wheelStats, 100, 10);
    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltaAngle, 0.0);
}

TEST(testodem, third) {
    WheelStats wheelStats{
            .wheel1roAn = M_PI / 4,
            .wheel2roAn = M_PI * 3 / 4,
            .wheel3roAn = M_PI * 5 / 4,
            .wheel4roAn = M_PI * 7 / 4
    };

    DeltaWheels deltaWheels{
            .wheel1An = 0.1,
            .wheel2An = 0.1,
            .wheel3An = 0.1,
            .wheel4An = 0.1
    };

    DeltaMove deltaMove = clcMove(deltaWheels, wheelStats, 100, 10);
    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltay, 0.0);
    EXPECT_LT(deltaMove.deltaAngle, 0.0);
}

TEST(testodem, fourth) {
    WheelStats wheelStats{
            .wheel1roAn = M_PI / 4,
            .wheel2roAn = M_PI * 3 / 4,
            .wheel3roAn = M_PI * 5 / 4,
            .wheel4roAn = M_PI * 7 / 4
    };

    DeltaWheels deltaWheels{
            .wheel1An = 0.1,
            .wheel2An = 0.1,
            .wheel3An = -0.1,
            .wheel4An = -0.1
    };

    DeltaMove deltaMove = clcMove(deltaWheels, wheelStats, 100, 10);
//    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltay, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltaAngle, 0.0);
}

TEST(testodem, fifth) {
    WheelStats wheelStats{
            .wheel1roAn = M_PI / 4,
            .wheel2roAn = M_PI * 3 / 4,
            .wheel3roAn = M_PI * 5 / 4,
            .wheel4roAn = M_PI * 7 / 4
    };

    DeltaWheels deltaWheels{
            .wheel1An = 0.1,
            .wheel2An = 0.1,
            .wheel3An = -0.1,
            .wheel4An = -0.1
    };

    DeltaMove deltaMove = clcMove(deltaWheels, wheelStats, 100, 10);
//    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltay, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltaAngle, 0.0);
}

TEST(testodem, sixth) {
    WheelStats wheelStats{
            .wheel1roAn = 0,
            .wheel2roAn = M_PI / 2,
            .wheel3roAn = M_PI,
            .wheel4roAn = M_PI * 3 / 2
    };

    DeltaWheels deltaWheels{
            .wheel1An = 0.1,
            .wheel2An = 0,
            .wheel3An = -0.1,
            .wheel4An = 0
    };

    DeltaMove deltaMove = clcMove(deltaWheels, wheelStats, 100, 10);
//    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltax, 0.0);
    EXPECT_DOUBLE_EQ(deltaMove.deltaAngle, 0.0);
}

TEST(testodom, seventh
) {
short ss = overflowDiff(-32768 - 10, -32768);
EXPECT_EQ(ss,
-10);

short dd = overflowDiff(32765 + 20, 32765);
EXPECT_EQ(dd,
20);

short no = overflowDiff(30, 20);
EXPECT_EQ(no,
10);
}