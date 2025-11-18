/* Polling Timer Example for STM32F411 BlackPill */
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

    /* 4. Configure TIM2: PSC and ARR (values chosen for example) */
    TIM2->PSC = 15999;     // prescaler
    TIM2->ARR = 999;    // auto-reload value
    TIM2->CNT = 0;       // reset counter

    /* 5. Enable counter */
    TIM2->CR1 |= TIM_CR1_CEN;

    while (1) {
        /* Toggle LED */
        GPIOC->ODR ^= (1 << 13);

        /* Wait for update event (UIF) — indicates CNT reached ARR */
        while (!(TIM2->SR & TIM_SR_UIF));
        TIM2->SR &= ~TIM_SR_UIF;  // clear UIF
    }

    return 0;
}

