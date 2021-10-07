/*------------------------------------------------------------------------------------------------------
 * I2C interface module
 *______________________________________________________________________________________________________
 *                      __    __ __ __     __   _____     _      ____
 *                      \ \  / //  \\ \    \_\ / ___/    | | 2  /  __\
 *                       \ \/ //    \\ \___ |/ \___ \    | |   |  |___
 *                        \__//__/\__\\____\   /____/    |_|    \____/
 *_______________________________________________________________________________________________________
 *
 * Created: 18-Sep-2021
 *  Author: VAL
 *------------------------------------------------------------------------------------------------------- 
 */

#include "interface.h"

I2C_ERR _i2c_init(void)
{
    //Enable i2c clocking
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
       
    //Enable GPIO alternate function
    //In this case I'm used I2C1, it tied to PB7, PB6
        
    //Enable PORTB clocking
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        
    //Set PB6 and PB7 to alternate function mode open drain
    GPIOB->CRL    |= (GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_1);   // Output max speed
    GPIOB->CRL    |= (GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0  | GPIO_CRL_CNF6_1  | GPIO_CRL_CNF7_1);    // Alternate function output Open-drain
                
    //I2C initialization
    I2C1->CR2   = 36;   //FREQ[5:0] /d36 periferal speed 36MHz Tpclk = 1/26MHz = 27.777[7]nS
    I2C1->CCR   = 180;  // Standart mode / CCR = 180 (to reach 100kHz speed)
    I2C1->TRISE = 37;   // 1000nS/27.7[7]nS + 1 (accordingly to reference manual) 1000nS - Rising edge of signal (from datasheet)
    I2C1->CR1   = ( I2C_CR1_PE); //Periferal enable
        
    //if pizdec start magic spell from errata))
    if(I2C1->SR2 == (I2C_SR2_BUSY) )
    {
        //Let begin out dance across blue pill (( 
        //1. Disable I2C peripheral
        I2C1->CR1   &=~I2C_CR1_PE;
            
        //2. Configure SCL & SDA as output open drain, hight
        GPIOB->CRL    |= (GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_1);   // Output max speed
        GPIOB->CRL    &=~(GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0  | GPIO_CRL_CNF6_1  | GPIO_CRL_CNF7_1);    // Clear old values
        GPIOB->CRL    |= (GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0);                                          // Output Open-drain
        GPIOB->ODR    |= (GPIO_ODR_ODR6 | GPIO_ODR_ODR7);                                               // Set high at PB6 & PB7
            
        //3. Check SCL & SDA high level
        if((GPIOB->IDR & (GPIO_IDR_IDR6 | GPIO_IDR_IDR7)) != (GPIO_ODR_ODR6 | GPIO_ODR_ODR7)) 
            return I2C_BUS_FAULT;
            
        //4. Config SDA to low level
        GPIOB->ODR    &=~( GPIO_ODR_ODR7 );
            
        //5. Check SDA low level
        if((GPIOB->IDR & (GPIO_IDR_IDR7))) //If high state
            return I2C_SDA_FAULT;
            
        //6. Config SCL to low level
        GPIOB->ODR    &=~( GPIO_ODR_ODR6 );
            
        //7. Check SCL low level
        if((GPIOB->IDR & (GPIO_IDR_IDR6))) //If high state
            return I2C_SCL_FAULT;
            
        //8. Config SCL to high level
        GPIOB->ODR    |= ( GPIO_ODR_ODR6 );
            
        //9. Check SCL high level
        if(!(GPIOB->IDR & (GPIO_IDR_IDR6))) //If low state
            return I2C_SCL_FAULT;
            
        //10. Config SDA to high level
        GPIOB->ODR    |= ( GPIO_ODR_ODR7 );
            
        //11. Check SDA high level
        if(!(GPIOB->IDR & (GPIO_IDR_IDR7))) //If low state
            return I2C_SDA_FAULT;
            
        GPIOB->ODR  &=~ ( GPIO_ODR_ODR6 | GPIO_ODR_ODR7 );
            
        //12. Configure SDA & SCL to alternate function open-drain
        GPIOB->CRL  &=~(GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0  | GPIO_CRL_CNF6_1  | GPIO_CRL_CNF7_1);
        GPIOB->CRL  |= (GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0  | GPIO_CRL_CNF6_1  | GPIO_CRL_CNF7_1);
            
        //13. Set SWRST bit in I2C_CR1 register
        I2C1->CR1   |= (I2C_CR1_SWRST);  
            
        //14. Clear SWRST bit in I2C_CR1 register
        I2C1->CR1   &=~(I2C_CR1_SWRST);
            
        //15. Enable I2C peripheral
        I2C1->CR1   |= I2C_CR1_PE;
            
        if(I2C1->SR2 == (I2C_SR2_BUSY) )
            return I2C_BUSY;
    }
        
    // The result of my tests - this magic spell is working)))
    return I2C_OK;
}
    
void _i2c_start_operation(void)
{
    //Generate start condition
    I2C1->CR1   |= I2C_CR1_START;
      
    //Wait till start condition ready
    while( !(I2C1->SR1 & (I2C_SR1_SB)) ){};
		(void) I2C1->SR1;
}
    
void _i2c_stop_operation(void)
{
    //Generate stop condition
    SET_BIT(I2C1->CR1 , I2C_CR1_STOP);
}
    
void _i2c_send_address(uint8_t byte)
{
	CLEAR_BIT(I2C1->CR1, I2C_CR1_POS);
	MODIFY_REG(I2C1->CR1, I2C_CR1_ACK, I2C_CR1_ACK);
	I2C1->DR = byte;
       
	volatile uint32_t test_reg;
	do
	{
		test_reg = I2C1->SR1;
	}while( !(test_reg & I2C_SR1_ADDR));
	(void) I2C1->SR1;
	(void) I2C1->SR2;
}
	
I2C_ERR _i2c_send_byte(uint8_t byte)
{
	I2C1->DR = byte;
       
	uint32_t test_reg;
	do
	{
		test_reg = I2C1->SR1;
		if(test_reg & I2C_SR1_ADDR)
			return I2C_OK;
		if(test_reg & I2C_SR1_TXE)
			return I2C_OK;
		if(test_reg & I2C_SR1_AF)
        {
            I2C1->SR1 &=~ I2C_SR1_AF;
            return(I2C_ACK_FAIL);
        }
			
	}while( !(test_reg & I2C_SR1_TXE) || !(test_reg & I2C_SR1_ADDR));

    return(I2C_OK);
}
    
uint8_t _i2c_receive_byte(void)
{
    (void)I2C1->SR1;
    (void)I2C1->SR2;
        
    while( !(I2C1->SR1 & I2C_SR1_RXNE) ){}
            
    return(I2C1->DR);
}
    
uint8_t _i2c_receive_last_byte(void)
{
    CLEAR_BIT(I2C1->CR1 , I2C_CR1_ACK);
    (void)I2C1->SR1;
    (void)I2C1->SR2;
        
    while( !(I2C1->SR1 & I2C_SR1_RXNE) ){}
            
    return(I2C1->DR);
}
