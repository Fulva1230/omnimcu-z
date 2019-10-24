#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_ll_bus.h>

//
// Created by Fulva on 10/21/2019.
//

#ifndef ENCODERHEHE
#define ENCODERHEHE

void EncoderInitialise(int dummy) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    GPIO_InitTypeDef gpioInitTypeDefO{
            .Pin = GPIO_PIN_0,
            .Mode=GPIO_MODE_AF_PP,
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


    TIM_HandleTypeDef timHandleTypeDef{
            .Instance=TIM2,
            .Init=TIM_Base_InitTypeDef{
                    .Prescaler=0x0000U,
                    .CounterMode=TIM_COUNTERMODE_CENTERALIGNED1,
                    .Period=0xFFFFU,
                    .ClockDivision=TIM_CLOCKDIVISION_DIV1,
                    .RepetitionCounter=0x00
            }
    };
    TIM_Encoder_InitTypeDef timEncoderInitTypeDef{
            .EncoderMode=TIM_ENCODERMODE_TI12,
            .IC1Polarity=TIM_ICPOLARITY_RISING,
            .IC1Selection=TIM_ICSELECTION_DIRECTTI,
            .IC1Prescaler=TIM_ICPSC_DIV8,
            .IC1Filter=0x0000,
            .IC2Polarity=TIM_ICPOLARITY_RISING,
            .IC2Selection=TIM_ICSELECTION_DIRECTTI,
            .IC2Prescaler=TIM_ICPSC_DIV8,
            .IC2Filter=0x0000
    };
    HAL_StatusTypeDef halStatusTypeDef = HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &timEncoderInitTypeDef
    );
    if (halStatusTypeDef == HAL_OK) {
        halStatusTypeDef = HAL_TIM_Encoder_Start(&timHandleTypeDef, TIM_CHANNEL_ALL);
    }
}

#endif