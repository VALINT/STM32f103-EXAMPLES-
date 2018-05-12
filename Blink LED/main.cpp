#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void delay(unsigned int nCount);

GPIO_InitTypeDef GPIO_InitStruct1;

int main (void)
{ 
    // Enable clock for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// Configure PB12 as push-pull output
	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct1);
	while (1)
	{
		/* Toggle LED on PB12 */
		// Reset bit will turn on LED (because the logic is interved)
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		delay(1000);
		// Set bit will turn off LED (because the logic is interved)
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
		delay(1000);
	}
}

// Delay function
void delay(unsigned int nCount)
{
	unsigned int i, j;
	
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 0x2AFF; j++);
}
