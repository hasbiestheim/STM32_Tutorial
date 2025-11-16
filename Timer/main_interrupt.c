/* main.c
   CMSIS example for STM32F411CE
   TIM2: PSC = 15999, ARR = 999 -> update interrupt used to toggle PC13.
   Assumes standard startup (SystemInit) provided by PlatformIO.
*/
#include "stm32f4xx.h"


/* The name of the IRQ function should match the one in the vector table in the startup file */
void TIM2_IRQHandler(void){
    /* Check what type of event occurred */
    if (TIM2->SR & TIM_SR_UIF)
    {
        /* Clear the interrupt even flag. CPU will only respond to new flags thereafter */
        TIM2->SR &= ~(TIM_SR_UIF);
        GPIOC->ODR ^= GPIO_ODR_OD13;
    }
}

int main(){
    /* Enable GPIOC Clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    /* Set Port C Pin 13 as Output */
    GPIOC->MODER &= ~(0x3UL << (13 * 2));
    GPIOC->MODER |=  (0x1UL << (13 * 2));
    // GPIOC->OTYPER &= ~(1 << 13);
    // GPIOC->OSPEEDR &= ~(0x3UL << (13 * 2));
    
    /* Enable Timer 2 peripheral */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* Default APB1 speed is 16MHz. */
    TIM2->PSC = 16000 - 1;
    /* Disable Timer*/
    TIM2->CR1 = 0;
    /* Reinitialize all registers */
    TIM2->EGR = TIM_EGR_UG;
    /* 1000 Counts at 1hz IRQ */
    TIM2->ARR = 1000 - 1;
    
    /* Enable hardware interrupts */
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->SR &= ~(1<<0);
    /* Start Timer */
    TIM2->CR1 |= TIM_CR1_CEN;

    /* These are CMSIS calls to enable interrupts */
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    // NVIC->IP[TIM2_IRQn] =  (2 << 4); // Set priority to 2
    // NVIC->ISER[TIM2_IRQn >> 5] |= (1 << (TIM2_IRQn % 32)); // Enable interrupt

    while (1){
        /* Do nothing here */
                /* Toggle LED */
    }
}
