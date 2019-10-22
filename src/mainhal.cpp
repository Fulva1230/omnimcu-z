#include <stm32f4xx_hal_tim.h>

//
// Created by Fulva on 10/21/2019.
//
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {

}

void EncoderInitialise() {
            __GPIOA_CLK_ENABLE();
            __GPIOB_CLK_ENABLE();
            __TIM2_CLK_ENABLE();
    TIM_HandleTypeDef timHandleTypeDef{.Instance=TIM2};
    TIM_Encoder_InitTypeDef timEncoderInitTypeDef{.EncoderMode=TIM_ENCODERMODE_TI12,
            .IC1Polarity=TIM_ICPOLARITY_BOTHEDGE,
            .IC2Polarity=TIM_ICPOLARITY_BOTHEDGE};
    HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &timEncoderInitTypeDef
    );
    HAL_TIM_Encoder_Start(&timHandleTypeDef,)
}