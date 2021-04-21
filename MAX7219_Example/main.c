#include "stm32f10x.h"
#include "stdint.h"
//#include "MAX7219.h"

#define SYSCLOCK 8000000UL

uint32_t volatile delay = 0;
void SysTick_Handler(void)      //SysTick IRQ Handler
{
    delay -= (uint32_t)(delay > 0);
}

void delay_ms(uint32_t ms);
void _SPI_send_byte(uint16_t data);
void _SPI_send_byte_in(uint16_t data);

uint8_t screen[8][3] = {
    {0xF2, 0xAC, 0x88},
    {0x8A, 0x32, 0x90},
    {0x8A, 0xA2, 0xA0},
    {0xF2, 0xA2, 0xC0},
    {0x8A, 0xA2, 0xA0},
    {0x8A, 0xA2, 0x90},
    {0xF2, 0xA2, 0x88},
    {0x00, 0x00, 0x00}
};

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

    
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // Set 5th (SPI1_SCK) and 7th (SPI1 MOSI) to AF_PP mode, speed 10MHz
    GPIOA->CRL   |= GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1 |  
                    GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1 | 
                    GPIO_CRL_MODE4_0;
    
    GPIOA->CRL   &=~GPIO_CRL_CNF7_0; 
    GPIOA->CRL   &=~GPIO_CRL_CNF5_0;
    GPIOA->CRL   &=~GPIO_CRL_CNF4_0;
    
    GPIOA->BSRR = GPIO_BSRR_BS4;
    
    //Enable SPI1 Clocking
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
    // Set-up SPI1 to Master mode
    SPI1->CR1  = SPI_CR1_BIDIMODE | 
                 SPI_CR1_BIDIOE | 
                 SPI_CR1_DFF | 
                 SPI_CR1_BR_2 | 
                 SPI_CR1_BR_1 | 
                 SPI_CR1_SSM | 
                 SPI_CR1_SSI | 
                 SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SPE;
 
    delay_ms(1000);
    
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0900);
    _SPI_send_byte_in(0x0900);
    _SPI_send_byte_in(0x0900);
    GPIOA->BSRR = GPIO_BSRR_BS4;
            
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0A01);
    _SPI_send_byte_in(0x0A01);
    _SPI_send_byte_in(0x0A01);        
    GPIOA->BSRR = GPIO_BSRR_BS4;

    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0B07);
    _SPI_send_byte_in(0x0B07);
    _SPI_send_byte_in(0x0B07);        
    GPIOA->BSRR = GPIO_BSRR_BS4;

    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0C01);
    _SPI_send_byte_in(0x0C01);
    _SPI_send_byte_in(0x0C01);      
    GPIOA->BSRR = GPIO_BSRR_BS4;

    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0F00);
    _SPI_send_byte_in(0x0F00);
    _SPI_send_byte_in(0x0F00);      
    GPIOA->BSRR = GPIO_BSRR_BS4;

    while(1)
    {
        for(uint16_t i = 1; i < 9; i++)
            {
                GPIOA->BSRR = GPIO_BSRR_BR4;    
                _SPI_send_byte_in((i<<8)| screen[i-1][0]);
                _SPI_send_byte_in((i<<8)| screen[i-1][1]);
                _SPI_send_byte_in((i<<8)| screen[i-1][2]);
                GPIOA->BSRR = GPIO_BSRR_BS4;
            }
        GPIOC->ODR    ^= GPIO_ODR_ODR13;
        delay_ms(2000);
        for(uint16_t i = 1; i < 9; i++)
            {
                GPIOA->BSRR = GPIO_BSRR_BR4;    
                _SPI_send_byte_in((i<<8));
                _SPI_send_byte_in((i<<8));
                _SPI_send_byte_in((i<<8));
                
                GPIOA->BSRR = GPIO_BSRR_BS4;
            }        
        GPIOC->ODR    ^= GPIO_ODR_ODR13;    
        delay_ms(2000);
    }
}

void delay_ms(uint32_t ms)      
{
    delay = ms;
    while(delay);
}

void _SPI_send_byte(uint16_t data)
{
    GPIOA->BSRR = GPIO_BSRR_BR4;
    
    SPI1->DR = data;
    while(!SPI1->SR & SPI_SR_TXE);
    while(SPI1->SR & SPI_SR_BSY);
    
    GPIOA->BSRR = GPIO_BSRR_BS4;
}
        
void _SPI_send_byte_in(uint16_t data)
{
    SPI1->DR = data;
    while(!SPI1->SR & SPI_SR_TXE);
    while(SPI1->SR & SPI_SR_BSY);
}
