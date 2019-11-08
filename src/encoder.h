#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_ll_bus.h>

//
// Created by Fulva on 10/21/2019.
//

#ifndef ENCODERHEHE
#define ENCODERHEHE

const GPIO_InitTypeDef ENCODER_GPIO_INIT{
        .Pin = GPIO_PIN_0,
        .Mode=GPIO_MODE_AF_PP,
        .Pull=GPIO_PULLUP,
        .Speed=GPIO_SPEED_FREQ_LOW,
        .Alternate=GPIO_AF1_TIM2
};

//TODO I should  change the prescaler and period to increase the accuracy of the motor encoder count and prepare for the counter overflow
const TIM_Base_InitTypeDef ENCODER_BASE_INIT{
        .Prescaler=0x0004U,
        .CounterMode=TIM_COUNTERMODE_UP,
        .Period=0xFFFFU,
        .ClockDivision=TIM_CLOCKDIVISION_DIV1,
        .RepetitionCounter=0x00
};

const TIM_Encoder_InitTypeDef TIM_ENCODER_INIT{
        .EncoderMode=TIM_ENCODERMODE_TI12,
        .IC1Polarity=TIM_ICPOLARITY_RISING,
        .IC1Selection=TIM_ICSELECTION_DIRECTTI,
        .IC1Prescaler=TIM_ICPSC_DIV1,
        .IC1Filter=0x0000,
        .IC2Polarity=TIM_ICPOLARITY_RISING,
        .IC2Selection=TIM_ICSELECTION_DIRECTTI,
        .IC2Prescaler=TIM_ICPSC_DIV1,
        .IC2Filter=0x0000
};

void EncoderInitialiseTIM1() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

    GPIO_InitTypeDef gpioInitTypeDef1{ENCODER_GPIO_INIT};
    gpioInitTypeDef1.Pin = GPIO_PIN_9;
    gpioInitTypeDef1.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOE, &gpioInitTypeDef1);

    GPIO_InitTypeDef gpioInitTypeDef2{ENCODER_GPIO_INIT};
    gpioInitTypeDef2.Pin = GPIO_PIN_11;
    gpioInitTypeDef2.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOE, &gpioInitTypeDef2);

    TIM_HandleTypeDef timHandleTypeDef{
            .Instance=TIM1,
            .Init=ENCODER_BASE_INIT
    };
    TIM_Encoder_InitTypeDef encoderInitTypeDef{TIM_ENCODER_INIT};
    HAL_StatusTypeDef halStatusTypeDef = HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &encoderInitTypeDef
    );
    if (halStatusTypeDef == HAL_OK) {
        HAL_TIM_Encoder_Start(&timHandleTypeDef, TIM_CHANNEL_ALL);
    }
}

void EncoderInitialiseTIM2() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    GPIO_InitTypeDef gpioInitTypeDef1{ENCODER_GPIO_INIT};
    gpioInitTypeDef1.Pin = GPIO_PIN_5;
    gpioInitTypeDef1.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &gpioInitTypeDef1);

    GPIO_InitTypeDef gpioInitTypeDef2{ENCODER_GPIO_INIT};
    gpioInitTypeDef2.Pin = GPIO_PIN_3;;
    gpioInitTypeDef2.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &gpioInitTypeDef2);

    TIM_HandleTypeDef timHandleTypeDef{
            .Instance=TIM2,
            .Init=ENCODER_BASE_INIT
    };
    TIM_Encoder_InitTypeDef encoderInitTypeDef{TIM_ENCODER_INIT};
    HAL_StatusTypeDef halStatusTypeDef = HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &encoderInitTypeDef
    );
    if (halStatusTypeDef == HAL_OK) {
        HAL_TIM_Encoder_Start(&timHandleTypeDef, TIM_CHANNEL_ALL);
    }
}

void EncoderInitialiseTIM3() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    GPIO_InitTypeDef gpioInitTypeDef1{ENCODER_GPIO_INIT};
    gpioInitTypeDef1.Pin = GPIO_PIN_4;
    gpioInitTypeDef1.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &gpioInitTypeDef1);

    GPIO_InitTypeDef gpioInitTypeDef2{ENCODER_GPIO_INIT};
    gpioInitTypeDef2.Pin = GPIO_PIN_5;
    gpioInitTypeDef2.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &gpioInitTypeDef2);

    TIM_HandleTypeDef timHandleTypeDef{
            .Instance=TIM3,
            .Init=ENCODER_BASE_INIT
    };
    TIM_Encoder_InitTypeDef encoderInitTypeDef{TIM_ENCODER_INIT};
    HAL_StatusTypeDef halStatusTypeDef = HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &encoderInitTypeDef
    );
    if (halStatusTypeDef == HAL_OK) {
        HAL_TIM_Encoder_Start(&timHandleTypeDef, TIM_CHANNEL_ALL);
    }
}

void EncoderInitialiseTIM4() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
    GPIO_InitTypeDef gpioInitTypeDef1{ENCODER_GPIO_INIT};
    gpioInitTypeDef1.Pin = GPIO_PIN_6;
    gpioInitTypeDef1.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB, &gpioInitTypeDef1);

    GPIO_InitTypeDef gpioInitTypeDef2{ENCODER_GPIO_INIT};
    gpioInitTypeDef2.Pin = GPIO_PIN_13;
    gpioInitTypeDef2.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &gpioInitTypeDef2);

    TIM_HandleTypeDef timHandleTypeDef{
            .Instance=TIM4,
            .Init=ENCODER_BASE_INIT
    };
    TIM_Encoder_InitTypeDef encoderInitTypeDef{TIM_ENCODER_INIT};
    HAL_StatusTypeDef halStatusTypeDef = HAL_TIM_Encoder_Init(
            &timHandleTypeDef, &encoderInitTypeDef
    );
    if (halStatusTypeDef == HAL_OK) {
        HAL_TIM_Encoder_Start(&timHandleTypeDef, TIM_CHANNEL_ALL);
    }
}

#endif