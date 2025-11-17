/* stm32f411_exti_pa0_pc13.c
   Bare-metal example: PA0 (button) -> EXTI0 -> toggle PC13 (LED)
   Compile with device headers (CMSIS) and startup files for STM32F411.
*/

#include "stm32f4xx.h"

/* Initialize PA0 as input and PC13 as output and EXTI0 */
void GPIO_EXTI_Init(void)
{
    /* 1) Enable GPIOA and GPIOC clocks and SYSCFG clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* 2) Configure PC13 as general purpose output, push-pull, low speed */
    /* MODER13 = 01 (output) */
    GPIOC->MODER &= ~(3UL << (13 * 2));
    GPIOC->MODER |=  (1UL << (13 * 2));
    /* OTYPER = 0 push-pull */
    GPIOC->OTYPER &= ~(1UL << 13);
    /* OSPEED = low (00) leave default */
    /* PUPDR = no pull-up/pull-down */
    GPIOC->PUPDR &= ~(3UL << (13 * 2));

    /* Initialize PC13 to HIGH or LOW as desired (on many boards LED is active low) */
    GPIOC->BSRR = (1UL << 13); // set PC13 (set pin high)

    /* 3) Configure PA0 as input with pull-down (to avoid floating). */
    GPIOA->MODER &= ~(3UL << (0 * 2));      // PA0 input (00)
    GPIOA->PUPDR &= ~(3UL << (0 * 2));
    GPIOA->PUPDR |=  (1UL << (0 * 2));      // 01 -> pull-down

    /* 4) Map EXTI0 line to PA0 via SYSCFG exticr[0] */
    /* Clear EXTI0 bits (bits [3:0]) then set to 0000 for PA[x] (PA = 0) */
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0; // maps PA0 (value 0), safe to write

    /* 5) Configure EXTI line 0:
       - unmask line in IMR
       - select falling or rising trigger
    */
    EXTI->IMR |= (1UL << 0);    // unmask EXTI0
    EXTI->EMR &= ~(1UL << 0);   // disable event, we'll use interrupt

    /* Choose trigger: falling edge (button pressed to ground->adjust depending HW)
       Use RTSR for rising, FTSR for falling. To enable both set both bits.
    */
    EXTI->FTSR |= (1UL << 0);   // falling edge trigger enabled
    EXTI->RTSR &= ~(1UL << 0);  // disable rising edge

    /* 6) Clear any pending EXTI0 flag just in case */
    EXTI->PR = (1UL << 0);

    /* 7) Configure NVIC: set priority and enable IRQ for EXTI0 */
    NVIC_SetPriority(EXTI0_IRQn, 1);   // priority level 1 (lower is higher priority)
    NVIC_EnableIRQ(EXTI0_IRQn);        // enable EXTI0 IRQ in NVIC
}

/* Simple debounce (blocking) - optional */
static void simple_delay(volatile uint32_t d) {
    while (d--) __asm("nop");
}

/* EXTI0 IRQ Handler: Toggle PC13 and clear pending flag */
void EXTI0_IRQHandler(void)
{
    /* Check pending bit for line 0 */
    if (EXTI->PR & (1UL << 0)) {
        /* clear pending by writing 1 */
        EXTI->PR = (1UL << 0);

        /* Optional: simple debounce */
        simple_delay(50000);

        //Toggle PC13:
        GPIOC->ODR ^= (1UL << 13); 
    }
}

/* main */
int main(void)
{
    /* Initialize GPIO and EXTI */
    GPIO_EXTI_Init();

    /* main loop */
    while (1) {
        /* do other work; LED toggles in ISR when PA0 triggers */
        __asm("wfi"); // wait for interrupt (low-power hint)
    }
}
