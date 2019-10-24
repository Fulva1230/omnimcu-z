//
// Created by fulva on 10/24/19.
//

#include <stm32f446xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_ll_bus.h>

volatile int countd = 0;

namespace speedcon {
    const TIM_Base_InitTypeDef baseInitTypeDef{
            .Prescaler=0x0000U,
            .CounterMode=TIM_COUNTERMODE_UP,
            .Period=0x0FFFU,
            .ClockDivision=TIM_CLOCKDIVISION_DIV1,
            .RepetitionCounter=0x00
    };

    TIM_HandleTypeDef TIM_HANDLETYPEDEF{
            .Instance=TIM6,
            .Init=baseInitTypeDef
    };


}

void testInterrupt() {
    NVIC_SetVector(TIM6_DAC_IRQn, (uint32_t) &HAL_TIM_IRQHandler);
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 4, 4);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);


    HAL_TIM_Base_Init(&speedcon::TIM_HANDLETYPEDEF);
    HAL_TIM_Base_Start_IT(&speedcon::TIM_HANDLETYPEDEF);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        ++countd;
        __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    }
}