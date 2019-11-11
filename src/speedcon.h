//
// Created by fulva on 10/24/19.
//

#include <src/MMotor.h>
#include <stm32f446xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_ll_bus.h>

#define TIM_USR TIM7
#define TIM_USR_IRQn TIM7_IRQn
#define COUNT_OF_MOTORS 4
#define PERIOD 100 //2 for 1ms

#define INPUT_FACTOR 0.01


namespace speedcon {
    MMotor *motors[COUNT_OF_MOTORS];
    const TIM_Base_InitTypeDef baseInitTypeDef{
            .Prescaler=44999,
            .CounterMode=TIM_COUNTERMODE_UP,
            .Period=50, //1 for 1ms, so 50 means update every 50ms
            .ClockDivision=TIM_CLOCKDIVISION_DIV1,
            .RepetitionCounter=0x00
    };

    TIM_HandleTypeDef TIM_HANDLETYPEDEF{
            .Instance=TIM_USR,
            .Init=baseInitTypeDef
    };


}

extern "C"
void M_TIM_USR_Handler(void) {
    if (__HAL_TIM_GET_FLAG(&speedcon::TIM_HANDLETYPEDEF, TIM_FLAG_UPDATE) == SET) {
        __HAL_TIM_CLEAR_FLAG(&speedcon::TIM_HANDLETYPEDEF, TIM_FLAG_UPDATE);
        for (MMotor *motor:speedcon::motors) {
            if (motor != nullptr) {
                motor->cSpeed = (motor->cPos - motor->prePos) * motor->countToRadian * 1000.0 /
                                PERIOD;//1000 for unit conversion, 2 for PERIOD conversion
                motor->prePos = motor->cPos;
                //if cSpeed is too big, that's because the counter overflow or underflow
                if (abs(motor->cSpeed) < 100) {
                    motor->speedErrorIg += motor->gSpeed - motor->cSpeed;
                    double speedErrorIgc = motor->speedErrorIg;
                    speedErrorIgc = min(1.0 / INPUT_FACTOR, speedErrorIgc);
                    speedErrorIgc = max(-1.0 / INPUT_FACTOR, speedErrorIgc);
                    motor->speedErrorIg = speedErrorIgc;
                    motor->drive(speedErrorIgc * INPUT_FACTOR);
                }
            }

        }

    }
}

void sppedconInit() {
    NVIC_SetVector(TIM_USR_IRQn, (uint32_t) M_TIM_USR_Handler);
//    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    HAL_NVIC_SetPriority(TIM_USR_IRQn, 15, 15);
    HAL_NVIC_EnableIRQ(TIM_USR_IRQn);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);


    HAL_TIM_Base_Init(&speedcon::TIM_HANDLETYPEDEF);
    HAL_TIM_Base_Start_IT(&speedcon::TIM_HANDLETYPEDEF);
}

