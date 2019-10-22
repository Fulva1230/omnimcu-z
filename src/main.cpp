#include <stm32f446xx.h>
#include <cstdio>
#include <mbed.h>

void EncoderInitialise(void) {

    //TIM2
    // configure GPIO PA15 & PB9 as inputs for Encoder 
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable clock for GPIOA
    GPIOA->MODER |= GPIO_MODER_MODER5_1;    // PA5 as Alternate Function
    GPIOA->OTYPER |= GPIO_OTYPER_OT_5;        // PA5 as Inputs
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;      // Low speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_0;// Pull Up
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0;        // AF01 for PA5
    GPIOA->AFR[1] |= GPIO_AFRH_AFRH0;        // AF01 for PA5

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;        // Enable clock for GPIOB
    GPIOB->MODER |= GPIO_MODER_MODER3_1; //PB3  as Alternate Function   /*!< GPIO port mode register,
    GPIOB->OTYPER |= GPIO_OTYPER_OT_3;    // PB3 as Inputs               /*!< GPIO port output type register,
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;// Low speed                        /*!< GPIO port output speed register
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR3_0;// Pull Up
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL3_0;
    GPIOB->AFR[1] |= GPIO_AFRH_AFRH0;         // AF01 for PB9
    //                                  /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */

    // configure TIM2 as Encoder input
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable clock for TIM2

    TIM2->CR1 = 0x0001;     // CEN(Counter ENable)='1'     < TIM control register 1
    TIM2->SMCR = 0x0003;     // SMS='011' (Encoder mode 3)  < TIM slave mode control register
    TIM2->CCMR1 = 0xF1F1;     // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
    TIM2->CCMR2 = 0x0000;     //                             < TIM capture/compare mode register 2
    TIM2->CCER = 0x0011;     // CC1E CC2E                   < TIM capture/compare enable register
    TIM2->PSC = 0x0000;     // Prescaler = (0+1)           < TIM prescaler
    TIM2->ARR = 0xffffffff; // reload at 0xfffffff         < TIM auto-reload register

    TIM2->CNT = 0x0000;  //reset the counter before we use it  

}


int main() {
    EncoderInitialise();


    unsigned int EncoderPositionTIM2;

    while (true) {
        // Print Encoder Quadrature count to debug port every 0.5 seconds
        EncoderPositionTIM2 = TIM2->CNT; // Get current position from Encoder

        printf("Encoder Position TIM2 %i\r\n  ", EncoderPositionTIM2);


        wait(0.5);
    }

}