#include "stm32f10x.h"
#include "stdint.h"

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
    // Initialization of SysTick timer
    SysTick->LOAD = SYSCLOCK/1000;
    SysTick->VAL  = SYSCLOCK/1000;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
    
    //Set-up GPIOC13 as PP
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH   |= GPIO_CRH_MODE13_0;

    // Set GPIOA5 (SPI1_SCK) and GPIOA7 (SPI1 MOSI) to AF_PP mode, speed 10MHz
    // Set GPIOA4 (SPI_CS) to GP_PP mode, speed 10MHz
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    GPIOA->CRL   |= GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1 |  
                    GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1 | 
                    GPIO_CRL_MODE4_0;
    GPIOA->CRL   &=~GPIO_CRL_CNF7_0; 
    GPIOA->CRL   &=~GPIO_CRL_CNF5_0;
    GPIOA->CRL   &=~GPIO_CRL_CNF4_0;
    
    //SPI_CS Set high
    GPIOA->BSRR = GPIO_BSRR_BS4;
    
    //Enable SPI1 Clocking
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
    // Set-up SPI1 to Master, without MISO, 16bit, soft NSS.
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
    
    //Init. MAX7219 as 8x8 matrices x 3 display
    //Decode mode - no decode
    //SPI_CS Set low
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0900);
    _SPI_send_byte_in(0x0900);
    _SPI_send_byte_in(0x0900);
    //SPI_CS Set high
    GPIOA->BSRR = GPIO_BSRR_BS4;
    
    //Intensity - 3/32
    //SPI_CS Set low    
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0A01);
    _SPI_send_byte_in(0x0A01);
    _SPI_send_byte_in(0x0A01);
    //SPI_CS Set high
    GPIOA->BSRR = GPIO_BSRR_BS4;
    
    //Scan limit - 7
    //SPI_CS Set low
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0B07);
    _SPI_send_byte_in(0x0B07);
    _SPI_send_byte_in(0x0B07);        
    //SPI_CS Set high
    GPIOA->BSRR = GPIO_BSRR_BS4;
    
    //Turn-on display
    //SPI_CS Set low
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0C01);
    _SPI_send_byte_in(0x0C01);
    _SPI_send_byte_in(0x0C01);      
    //SPI_CS Set high
    GPIOA->BSRR = GPIO_BSRR_BS4;
    
    //Normal operation mode
    //SPI_CS Set low
    GPIOA->BSRR = GPIO_BSRR_BR4;
    _SPI_send_byte_in(0x0F00);
    _SPI_send_byte_in(0x0F00);
    _SPI_send_byte_in(0x0F00);      
    //SPI_CS Set high
    GPIOA->BSRR = GPIO_BSRR_BS4;

    while(1)
    {
        for(uint16_t i = 1; i < 9; i++)
        {
            //SPI_CS Set low
            GPIOA->BSRR = GPIO_BSRR_BR4;    
            _SPI_send_byte_in((i<<8)| screen[i-1][0]);
            _SPI_send_byte_in((i<<8)| screen[i-1][1]);
            _SPI_send_byte_in((i<<8)| screen[i-1][2]);
            //SPI_CS Set high
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
            //SPI_CS Set high
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
