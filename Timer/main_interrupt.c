/* Interrupt Timer Example for STM32F411 BlackPill */
/* main_polling.c
   Bare-metal blink using TIM2 on STM32F411 (BlackPill).
   Polls TIM2 update flag (UIF) for a ~1s delay.
*/
#define STM32F411xE
#include "stm32f4xx.h"
#include "stm32f411xe.h"

int main(void) {
    /* 1. Enable GPIOC clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* 2. Configure PC13 as general purpose output (01) */
    GPIOC->MODER &= ~(0x3UL << (13 * 2));
    GPIOC->MODER |=  (0x1UL << (13 * 2));
    GPIOC->OTYPER &= ~(1 << 13);
    GPIOC->OSPEEDR &= ~(0x3UL << (13 * 2));
    GPIOC->PUPDR &= ~(0x3UL << (13 * 2));

    /* 3. Enable TIM2 clock (APB1) */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* 4. Configure TIM2: PSC and ARR for 1s period */
    TIM2->PSC = 49999;    // prescaler for 1000 Hz
    TIM2->ARR = 999;      // auto-reload value for 1s
    TIM2->CNT = 0;       // reset counter

    /* 5. Enable update interrupt */
    TIM2->DIER |= TIM_DIER_UIE;

    /* 6. Enable NVIC for TIM2 */
    NVIC_EnableIRQ(TIM2_IRQn);

    /* 7. Enable counter */
    TIM2->CR1 |= TIM_CR1_CEN;

    while (1) {
        // Main loop does nothing; LED toggled in interrupt
        // Add your code here
    }

    return 0;
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;  // clear UIF
        GPIOC->ODR ^= (1 << 13);  // toggle LED PORTC13
    }
}

