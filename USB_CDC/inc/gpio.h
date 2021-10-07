#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"
#include "core_cm3.h"

#define	PORTA			GPIOA
#define	PORTB			GPIOB
#define	PORTC			GPIOC
#define	PORTD			GPIOD
#define	PORTE			GPIOE
#define	PORTF			GPIOF
#define	PORTG			GPIOG

#define PIN0			0
#define PIN1			1
#define PIN2			2
#define PIN3			3
#define PIN4			4
#define PIN5			5
#define PIN6			6
#define PIN7			7
#define PIN8			8
#define PIN9			9
#define PIN10			10
#define PIN11			11
#define PIN12			12
#define PIN13			13
#define PIN14			14
#define PIN15			15

#define PIN0_REG		0
#define PIN1_REG		1
#define PIN2_REG		2
#define PIN3_REG		3
#define PIN4_REG		4
#define PIN5_REG		5
#define PIN6_REG		6
#define PIN7_REG		7
#define PIN8_REG		0
#define PIN9_REG		1
#define PIN10_REG		2
#define PIN11_REG		3
#define PIN12_REG		4
#define PIN13_REG		5
#define PIN14_REG		6
#define PIN15_REG		7

#define PIN0_SHIFT_N	0
#define PIN1_SHIFT_N	4
#define PIN2_SHIFT_N	8
#define PIN3_SHIFT_N	12
#define PIN4_SHIFT_N	16
#define PIN5_SHIFT_N	20
#define PIN6_SHIFT_N	24
#define PIN7_SHIFT_N	28
#define PIN8_SHIFT_N	0
#define PIN9_SHIFT_N	4
#define PIN10_SHIFT_N	8
#define PIN11_SHIFT_N	12
#define PIN12_SHIFT_N	16
#define PIN13_SHIFT_N	20
#define PIN14_SHIFT_N	24
#define PIN15_SHIFT_N	28

#define M_INPUT			0x0
#define M_OUT_2M		0x2
#define M_OUT_10M		0x1
#define	M_OUT_50M		0x3

#define C_ANALOG		0x0
#define C_FLOATING_IN	0x1
#define C_IN_PP_PD		0x2

#define C_GP_PP			0x0
#define C_GP_OD			0x1
#define	C_AF_PP			0x2
#define	C_AF_OD			0x3

#define INPUT_FLOATING			((C_FLOATING_IN << 2) | M_INPUT)
#define	INPUT_PULL_UP			((C_IN_PP_PD << 2) | M_INPUT)
#define	INPUT_PULL_DOWN			((C_IN_PP_PD << 2) | M_INPUT)
#define	ANALOG_MODE				((C_ANALOG << 2) | M_INPUT)
#define	OUTPUT_OPEN_DRAIN_2M	((C_GP_OD << 2) | M_OUT_2M)
#define	OUTPUT_PUSH_PULL_2M		((C_GP_PP << 2) | M_OUT_2M)
#define	OUTPUT_OPEN_DRAIN_10M	((C_GP_OD << 2) | M_OUT_10M)
#define	OUTPUT_PUSH_PULL_10M	((C_GP_PP << 2) | M_OUT_10M)
#define	OUTPUT_OPEN_DRAIN_50M	((C_GP_OD << 2) | M_OUT_50M)
#define	OUTPUT_PUSH_PULL_50M	((C_GP_PP << 2) | M_OUT_50M)
#define	ALTERN_PUSH_PULL_2M		((C_AF_PP << 2) | M_OUT_2M)
#define	ALTERN_OPEN_DRAIN_2M    ((C_AF_OD << 2) | M_OUT_2M)
#define	ALTERN_PUSH_PULL_10M    ((C_AF_PP << 2) | M_OUT_10M)
#define	ALTERN_OPEN_DRAIN_10M   ((C_AF_OD << 2) | M_OUT_10M)
#define	ALTERN_PUSH_PULL_50M    ((C_AF_PP << 2) | M_OUT_50M)
#define	ALTERN_OPEN_DRAIN_50M   ((C_AF_OD << 2) | M_OUT_50M)

#define ENABLE_PORT(port)				RCC->APB2ENR |= RCC_APB2ENR_IOP##port##EN
#define	CLEAR_REG_FULL(port,reg)		port->reg = 0;
#define CLEAR_REG_PART(port, reg, val)	port->reg &=~ val;
#define SET_REG_PART(port, reg, val)	port->reg |= val;
#define INIT_LOW_PIN(port, pin, func)	port->CRL |= (func << ( pin * 4 ))
#define INIT_HIGH_PIN(port, pin, func)	port->CRH |= (func << ((pin - 8 ) * 4))
#define CLEAR_INIT_LOW(port, pin)		port->CRL &=~ (0xf << ( pin * 4 ))
#define CLEAR_INIT_HIGH(port, pin)		port->CRH &=~ (0xf << ((pin - 8 ) * 4))
#define RESET_PIN(port, pin)			port->BRR = (0x1 << pin )
#define CLEAR_PIN(port, pin)			port->BSRR = (0x100 << pin)
#define SET_PIN(port, pin)				port->BSRR = (0x1 << pin)
#define INVERT_PIN(port, pin)			port->ODR = port->ODR^(0x1 << pin)
#define	READ_PIN(port, pin)				port_>IDR & (1 << pin)


#define	SPI1NSS				4
#define	SPI1SCK				5
#define	SPI1MISO			6
#define SPI1MOSI			7

#define	SPI1NSS_REG			4
#define	SPI1SCK_REG			5
#define	SPI1MISO_REG		6
#define SPI1MOSI_REG		7

#define	SPI1NSS_SHIFT_N		16
#define	SPI1SCK_SHIFT_N		20
#define	SPI1MISO_SHIFT_N	24
#define SPI1MOSI_SHIFT_N	28

#define	SPI2NSS				12
#define	SPI2SCK				13
#define	SPI2MISO			14
#define SPI2MOSI			15

#define	SPI2NSS_REG			4
#define	SPI2SCK_REG			5
#define	SPI2MISO_REG		6
#define SPI2MOSI_REG		7

#define	SPI2NSS_SHIFT_N		16
#define	SPI2SCK_SHIFT_N		20
#define	SPI2MISO_SHIFT_N	24
#define SPI2MOSI_SHIFT_N	28

#endif
