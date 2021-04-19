#include "stm32f10x.h"
#include "stdint.h"

#define SYSCLOCK 8000000UL

uint32_t volatile delay = 0;
void SysTick_Handler(void)      //SysTick IRQ Handler
{
    delay -= (uint32_t)(delay > 0);
}

void delay_ms(uint32_t ms);

int main(void)
{    
    // Initializate SysTick timer
    SysTick->LOAD = SYSCLOCK/1000;
    SysTick->VAL  = SYSCLOCK/1000;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
    
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH   |= GPIO_CRH_MODE13_0;
    
    while(1)
    {
        GPIOC->ODR    ^= GPIO_ODR_ODR13;
        delay_ms(500);
    }
}

void delay_ms(uint32_t ms)      
{
    delay = ms;
    while(delay);
}
