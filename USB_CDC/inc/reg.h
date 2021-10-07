#ifndef _REG_H_
#define _REG_H_


/*----------------------------------------------------------------------------------------------------------------------
 *	SPI REGISTERS DEFINE
 *--------------------------------------------------------------------------------------------------------------------*/
	//CPHA
	#define SPI_FIRST_CLOCK_PHASE	0x0000
	#define SPI_SECOND_CLOCK_PHASE	0x0001
	//CPOL
	#define SPI_CLK_POLARITY_HIGH	0x0002
	#define SPI_CLK_POLARITY_LOW	0x0000
	//MSTR	
	#define SPI_MASTER_MODE			0x0004
	#define SPI_SLAVE_MODE			0x0000
	//BR
	#define SPI_CLS_DIV_2			0x0000
	#define SPI_CLS_DIV_4			0x0008
	#define SPI_CLS_DIV_8			0x0010
	#define SPI_CLS_DIV_16			0x0018
	#define SPI_CLS_DIV_32			0x0020
	#define SPI_CLS_DIV_64			0x0028
	#define SPI_CLS_DIV_128			0x0030
	#define SPI_CLS_DIV_256			0x0038
	//SPE
	#define SPI_ENABLE				0x0040
	#define SPI_DISABLE				0x0000
	//LSBFIRST
	#define SPI_MSB_FIRST			0x0000
	#define SPI_LSB_FIRST			0x0080
	//SSI
	#define SPI_NSS_SOFT_EN			0x0100
	#define SPI_NSS_SOFT_DIS		0x0000
	//SSM
	#define SPI_SOFT_SLAVE_EN		0x0200
	#define SPI_SOFT_SLAVE_SID 	 	0x0000
	//RXONLY
	#define	SPI_FULL_DUPLEX			0x0000
	#define SPI_HALF_DUPLEX			0x0400
	//DFF
	#define SPI_8_BIT_FRAME			0x0000
	#define SPI_16_BIT_FRAME		0x0800
	//CRCNEXT
	#define SPI_DATA_PHASE			0x0000
	#define SPI_CRC_PHASE			0x1000
	//CRCEN
	#define SPI_COUNT_CRC_EN		0x2000
	#define SPI_COUNT_CRC_DIS		0x0000
	//BIDIOE
	#define SPI_RECEIVE_ONLY		0x0000
	#define SPI_TRANSMIT_EN			0x4000
	//BIDIMODE
	#define SPI_2_WIRES_UNIDIR		0x0000
	#define SPI_1_WIRES_BIDIR		0x8000

	//CR2
	#define SPI_RX_DMA_INT_EN		0x01
	#define SPI_TX_DMA_INT_EN		0x02
	#define SPI_SS_OUT_INT_EN		0x04
	#define SPI_ERROR_INT_EN		0x10
	#define SPI_RX_INT_EN			0x20
	#define SPI_TX_INT_EN			0x40

	//SR
	#define SPI_DATA_RECEIVED		0x01
	#define SPI_DATA_TRANSMITED		0x02
	#define SPI_UNDERRUN_FLAG		0x08
	#define SPI_CRC_ERROR			0x10
	#define SPI_MODE_FAULT			0x20
	#define SPI_OVERRUN_FLAG		0x40
	#define SPI_BUSY				0x80


#endif