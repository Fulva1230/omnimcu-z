//
// Created by fulva on 10/24/19.
//

#include <stm32f446xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_ll_bus.h>

volatile int countd = 0;

void testInterrupt() {
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    const TIM_Base_InitTypeDef baseInitTypeDef{
            .Prescaler=0x0000U,
            .CounterMode=TIM_COUNTERMODE_UP,
            .Period=0x0FFFU,
            .ClockDivision=TIM_CLOCKDIVISION_DIV1,
            .RepetitionCounter=0x00
    };

    TIM_HandleTypeDef timHandleTypeDef{
            .Instance=TIM6,
            .Init=baseInitTypeDef
    };
    HAL_TIM_Base_Init(&timHandleTypeDef);
    HAL_TIM_Base_Start_IT(&timHandleTypeDef);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        ++countd;
        __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    }
}