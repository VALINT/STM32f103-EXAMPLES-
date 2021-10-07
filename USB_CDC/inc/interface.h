/*------------------------------------------------------------------------------------------------------
 * Interface module
 *______________________________________________________________________________________________________
 *  __    __ __ __     __   _____     _  __  _  _____  ____  ____  ____    _    ____  ____
 *  \ \  / //  \\ \    \_\ / ___/    | ||  \| ||_   _|| ___|| |\ \| ___|  / \  /  __\| ___|
 *   \ \/ //    \\ \___ |/ \___ \    | || |\  |  | |  | ___|| |/ /| ___| / _ \ | |__ | ___|
 *    \__//__/\__\\____\   /____/    |_||_| \_|  |_|  |____||_|\_\|_|   /_/ \_\\____/|____|
 *_______________________________________________________________________________________________________
 *
 * Created: 18-Sep-2018 23:05:14
 *  Author: VAL
 *
 *------------------------------------------------------------------------------------------------------- 
 *	This module consist program realization of communication interfaces.
 *	Using hardware interfaces (need for easy programing if you need using interface)
 *-------------------------------------------------------------------------------------------------------
 *	History:
 *		
 *-------------------------------------------------------------------------------------------------------
 *	Features:
 *		-	Using hardware I2C interface
 * 		-	Using hardware USB 2.0 interface
 *      -   Using hardware SPI interface
 *      -   Using hardware UART interface 
 *
 *-------------------------------------------------------------------------------------------------------
 */

#ifndef STANDART_INTERFACE_H_
#define STANDART_INTERFACE_H_

#include "stm32f10x.h"
#include "core_cm3.h"
#include "stdint.h"

#include <stdint.h>
#include "gpio.h"
#include "reg.h"
#include "usb_def.h"

//-------------------------------------------------------------------------------------------------------
//	UART/USART HARDWARE part
//-------------------------------------------------------------------------------------------------------

#ifndef BAUD	//If baudrate not is not defined, set default value
	#define BAUD 9600 // Baud rate
#endif

//Procedures and functions needed for this interface work. 

void _uart_init(void);    
void _uart_write_char(char data);    
void _uart_send_array(char* data);    
void _uart_send_array_s(const char* data);

//-------------------------------------------------------------------------------------------------------
//	USB 2.0 HARDWARE part
//-------------------------------------------------------------------------------------------------------

void _usb_init(void);
void usb_set_ep(uint8_t ep_num, uint8_t ep_type, uint16_t txsz, uint16_t rxsz, void (*func)(usb_ep_t ep));
void usb_ep_write(uint8_t number, const uint8_t *buf, uint16_t size);
int usb_read_ep(uint8_t number, uint16_t *buf);

//-------------------------------------------------------------------------------------------------------
//	I2C HARDWARE part
//-------------------------------------------------------------------------------------------------------

//Procedures and functions needed for this interface work. 
typedef enum {I2C_OK, I2C_BUSY, I2C_SCL_FAULT, I2C_SDA_FAULT, I2C_BUS_FAULT, I2C_ACK_FAIL} I2C_ERR;

I2C_ERR _i2c_init(void);
void _i2c_start_operation(void);
void _i2c_stop_operation(void);
void _i2c_send_address(uint8_t byte);
I2C_ERR _i2c_send_byte(uint8_t byte);
uint8_t _i2c_receive_byte(void);
uint8_t _i2c_receive_last_byte(void);

//-------------------------------------------------------------------------------------------------------
//	SPI HARDWARE part
//-------------------------------------------------------------------------------------------------------
 
//Procedures and functions needed for work with this interface.     
#define _spi_disableSS_m        SET_PIN(GPIOB, SPI2NSS);//
#define _spi_enableSS_m         RESET_PIN(GPIOB, SPI2NSS);//
#define _spi_statSS_m       
#define _spi_writeByte_m(data)  do{_spi_send_byte(SPI2, data);}while(0)
#define _spi_readByte_m   		do{data = _spi_read_byte(SPI2);}while(0)
    
// Init SPI module accordingly to defined parameters above 
void _spi_init(void); 
void _spi_writeByte(uint8_t data); 
void _spi_send_byte(SPI_TypeDef*, char);
char _spi_read_byte(SPI_TypeDef*);
uint8_t _spi_readByte(void);
uint8_t _spi_writeReadByte(uint8_t data);
void _spi_writeArray(uint8_t num, uint8_t *data);
void _spi_writeReadArray(uint8_t num, uint8_t *data);
    
#endif /* STANDART_INTERFACE_H_ */
