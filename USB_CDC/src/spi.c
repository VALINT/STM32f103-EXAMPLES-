/*------------------------------------------------------------------------------------------------------
 * SPI interface module
 *______________________________________________________________________________________________________
 *                      __    __ __ __     __   _____     _____  ____   _
 *                      \ \  / //  \\ \    \_\ / ___/    / ___/ | __ \ | |
 *                       \ \/ //    \\ \___ |/ \___ \    \___ \ |  _ / | |
 *                        \__//__/\__\\____\   /____/    /____/ |_|    |_|
 *_______________________________________________________________________________________________________
 *
 * Created: 18-Sep-2021
 *  Author: VAL
 *------------------------------------------------------------------------------------------------------- 
 */

#include "interface.h"

void _spi_init()
{
    //Enable SPI1 Clocking
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; 
	//
    SPI2->CR1  = SPI_2_WIRES_UNIDIR | 
                 SPI_FULL_DUPLEX | 
                 SPI_8_BIT_FRAME | 
                 SPI_SOFT_SLAVE_EN | 
                 SPI_NSS_SOFT_EN|
                 SPI_FIRST_CLOCK_PHASE | 
                 SPI_CLK_POLARITY_LOW | 
                 SPI_CLS_DIV_32 | 
                 SPI_MASTER_MODE |
                 SPI_MSB_FIRST;
    SPI2->CR1 |= SPI_CR1_SPE;
}
	
void _spi_send_byte(SPI_TypeDef* SPI, char data)
{
	SPI->DR = data;
	while(!(SPI->SR & SPI_SR_TXE));
	while(SPI->SR & SPI_SR_BSY);
}
	
char _spi_read_byte(SPI_TypeDef* SPI)
{
	SPI->DR = 0xff;
	while(!(SPI->SR & SPI_SR_TXE));
	while(SPI->SR & SPI_SR_BSY);
	
    return((uint8_t)SPI->DR);
}
    
void _spi_writeByte(uint8_t data)
{
    SPI2->DR = data;
	while(!(SPI2->SR & SPI_SR_TXE));
	while(SPI2->SR & SPI_SR_BSY);    
}
	
uint8_t _spi_readByte(void)
{
    SPI2->DR = 0xff;
	while(!(SPI2->SR & SPI_SR_TXE));
	while(SPI2->SR & SPI_SR_BSY);       
    
	return((uint8_t)SPI2->DR);
}
    //
uint8_t _spi_writeReadByte(uint8_t data)
{
    SPI2->DR = data;
	while(!(SPI2->SR & SPI_SR_TXE));
	while(SPI2->SR & SPI_SR_BSY);       
    
	return((uint8_t)SPI2->DR);
}

void _spi_writeArray(uint8_t num, uint8_t *data)
{
	while(num--){
		SPI2->DR = *data;
		while(!(SPI2->SR & SPI_SR_TXE));
		while(SPI2->SR & SPI_SR_BSY);
        
		data++;
   }
}
    //
void _spi_writeReadArray(uint8_t num, uint8_t *data)
{
	while(num--){
		SPI2->DR = *data;
		while(!(SPI2->SR & SPI_SR_TXE));
		while(SPI2->SR & SPI_SR_BSY);
		
        *data = ((uint8_t)SPI2->DR); 
        data++;           
   }
}
