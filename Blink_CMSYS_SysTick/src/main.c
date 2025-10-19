#include "stm32f103xb.h"
#include "stdint.h"

#define SYSCLOCK 72000000UL


uint32_t volatile delay = 0;
void SysTick_Handler(void)      //SysTick IRQ Handler
{
    delay -= (uint32_t)(delay > 0);
}

void SystemClock_Config(void)
{
    // 1. Enable HSE
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0);  // Wait for HSE ready

    // 2. Set flash latency (2 wait states for 72 MHz)
    FLASH->ACR |= FLASH_ACR_PRFTBE;           // Enable prefetch buffer
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2;        // 2 WS

    // 3. Configure prescalers
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;          // AHB = SYSCLK /1 = 72 MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;         // APB1 = SYSCLK /2 = 36 MHz
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;         // APB2 = SYSCLK /1 = 72 MHz

    // 4. Configure PLL (HSE * 9 = 72 MHz)
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);

    // 5. Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);   // Wait for PLL ready

    // 6. Select PLL as system clock source
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Wait for PLL to be used

    // 7. (Optional) Update SystemCoreClock variable
    SystemCoreClockUpdate();
}

void delay_ms(uint32_t ms);

int main(void)
{    
    SystemClock_Config();
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
