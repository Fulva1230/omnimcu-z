#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>

//
// Created by Fulva on 10/21/2019.
//
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim) {

}

void EncoderInitialise(int dummy) {
            __GPIOA_CLK_ENABLE();
            __GPIOB_CLK_ENABLE();
            __TIM2_CLK_ENABLE();
    GPIO_InitTypeDef gpioInitTypeDefO{
            .Mode=GPIO_MODE_AF_OD,
            .Pull=GPIO_PULLUP,
            .Speed=GPIO_SPEED_FREQ_LOW,
            .Alternate=GPIO_AF1_TIM2
    };

    GPIO_InitTypeDef gpioInitTypeDef1{gpioInitTypeDefO};
    gpioInitTypeDef1.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOA, &gpioInitTypeDef1);

    GPIO_InitTypeDef gpioInitTypeDef2{gpioInitTypeDefO};
    gpioInitTypeDef2.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOB, &gpioInitTypeDef2);

    TIM_HandleTypeDef timHandleTypeDef{.Instance=TIM2};
    TIM_Encoder_InitTypeDef timEncoderInitTypeDef{
            .EncoderMode=TIM_ENCODERMODE_TI12,
            .IC1Polarity=TIM_ICPOLARITY_BOTHEDGE,
            .IC2Polarity=TIM_ICPOLARITY_BOTHEDGE};
    HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &timEncoderInitTypeDef
    );
    HAL_TIM_Encoder_Start(&timHandleTypeDef, TIM_CHANNEL_ALL);
}