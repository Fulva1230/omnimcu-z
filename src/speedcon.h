//
// Created by fulva on 10/24/19.
//

#include <src/MMotor.h>
#include <stm32f446xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_ll_bus.h>
#include <Twist.h>
#include "PDController.h"

#define TIM_USR TIM7
#define TIM_USR_IRQn TIM7_IRQn
#define COUNT_OF_MOTORS 4


#define INPUT_FACTOR 0.01

constexpr double PERIOD_IN_SEC{PERIOD / 1000.0};
const std::string FEEDBACK_MODE = "feedback";
const std::string OPENLOOP_MODE = "openloop";


namespace speedcon {
    MMotor *motors[COUNT_OF_MOTORS];
    const TIM_Base_InitTypeDef baseInitTypeDef{
            .Prescaler=44999,
            .CounterMode=TIM_COUNTERMODE_UP,
            .Period=PERIOD * 2, //1 for 1ms, so 100 means update every 100ms
            .ClockDivision=TIM_CLOCKDIVISION_DIV1,
            .RepetitionCounter=0x00
    };

    TIM_HandleTypeDef TIM_HANDLETYPEDEF{
            .Instance=TIM_USR,
            .Init=baseInitTypeDef
    };


}

extern "C"
void FEEDBACKCON(void) {
    if (__HAL_TIM_GET_FLAG(&speedcon::TIM_HANDLETYPEDEF, TIM_FLAG_UPDATE) == SET) {
        __HAL_TIM_CLEAR_FLAG(&speedcon::TIM_HANDLETYPEDEF, TIM_FLAG_UPDATE);
        for (MMotor *motor:speedcon::motors) {
            if (motor != nullptr) {
                motor->cSpeed = (motor->cPos - motor->prePos) * motor->countToRadian /
                                PERIOD_IN_SEC;//1000 for unit conversion
                motor->prePos = motor->cPos;
                //if cSpeed is too big, that's because the counter overflow or underflow
                if (abs(motor->cSpeed) < 100) {
                    motor->speedErrorIg += motor->gSpeed - motor->cSpeed;
                    double speedErrorIgc = motor->speedErrorIg;
                    speedErrorIgc = min(1.0 / INPUT_FACTOR, speedErrorIgc);
                    speedErrorIgc = max(-1.0 / INPUT_FACTOR, speedErrorIgc);
                    motor->speedErrorIg = speedErrorIgc;
                    motor->pDcon.setInput(speedErrorIgc);
                    motor->drive(motor->pDcon.getOutput() * INPUT_FACTOR);
                }
            }

        }

    }
}

extern "C" void OPENLOOPCON(void) {
    if (__HAL_TIM_GET_FLAG(&speedcon::TIM_HANDLETYPEDEF, TIM_FLAG_UPDATE) == SET) {
        __HAL_TIM_CLEAR_FLAG(&speedcon::TIM_HANDLETYPEDEF, TIM_FLAG_UPDATE);
        for (MMotor *motor:speedcon::motors) {
            if (motor != nullptr) {
                motor->drive(motor->gSpeed);
            }
        }
    }
}

void openloopconInit() {
    HAL_TIM_Base_Stop_IT(&speedcon::TIM_HANDLETYPEDEF);
    NVIC_SetVector(TIM_USR_IRQn, (uint32_t) OPENLOOPCON);
    HAL_TIM_Base_Start_IT(&speedcon::TIM_HANDLETYPEDEF);
}

void feedbackconInit() {
    HAL_TIM_Base_Stop_IT(&speedcon::TIM_HANDLETYPEDEF);
    NVIC_SetVector(TIM_USR_IRQn, (uint32_t) FEEDBACKCON);
    HAL_TIM_Base_Start_IT(&speedcon::TIM_HANDLETYPEDEF);
}

void fbGoalUpdate(const geometry_msgs::Twist &twist) {
    reference_wrapper<Wheel> wheels[] = {wheel1, wheel2, wheel3, wheel4};
    for (Wheel &wheel: wheels) {
        wheel.motor.gSpeed =
                (sin(wheel.theta) * twist.linear.x
                 - cos(wheel.theta) * twist.linear.y
                 - wheel.disToC * twist.angular.z) / wheel.radii;
    }
}

namespace speedcon {
    void mode_change(const std::string &mode) {
        if (mode == FEEDBACK_MODE) {
            feedbackconInit();
        } else if (mode == OPENLOOP_MODE) {
            openloopconInit();
        }
    }
}

void speedonInit() {
    modesubscribers.emplace_back(&speedcon::mode_change);
    HAL_NVIC_SetPriority(TIM_USR_IRQn, 15, 15);
    HAL_NVIC_EnableIRQ(TIM_USR_IRQn);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);
    HAL_TIM_Base_Init(&speedcon::TIM_HANDLETYPEDEF);
}
