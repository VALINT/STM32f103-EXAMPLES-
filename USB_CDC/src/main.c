#include "stm32f10x.h"
#include "core_cm3.h"
#include "interface.h"
#include "gpio.h"

int main(void)
{	
	uint8_t usb_ok = 0;
	
	// Enable PORTA clocking for USB
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	// Enable PORTC clocking rot LED (PC13)
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	// Init GPIO
	INIT_HIGH_PIN(GPIOC, 13, OUTPUT_PUSH_PULL_50M);
	INIT_HIGH_PIN(GPIOC, 12, OUTPUT_PUSH_PULL_50M);
	INIT_HIGH_PIN(GPIOC, 11, OUTPUT_PUSH_PULL_50M);
	INIT_HIGH_PIN(GPIOC, 10, OUTPUT_PUSH_PULL_50M);
	INIT_HIGH_PIN(GPIOC,  9, OUTPUT_PUSH_PULL_50M);
	INIT_HIGH_PIN(GPIOC,  8, OUTPUT_PUSH_PULL_50M);
	
	_usb_init();
	
	while(1){}
}
