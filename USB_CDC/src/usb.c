/*------------------------------------------------------------------------------------------------------
 * USB 2.0 interface module
 *______________________________________________________________________________________________________
 *                      __    __ __ __     __   _____     _   _   _____  ____ 
 *                      \ \  / //  \\ \    \_\ / ___/    | | | | / ___/ | __ \
 *                       \ \/ //    \\ \___ |/ \___ \    | |_| | \___ \ | __ \
 *                        \__//__/\__\\____\   /____/     \___/  /____/ |____/
 *_______________________________________________________________________________________________________
 *
 * Created: 18-Sep-2021
 *  Author: VAL
 *------------------------------------------------------------------------------------------------------- 
 *  Acknowledgment:
 *  Based on sources from @saewave and @eddyem
 * 
 *------------------------------------------------------------------------------------------------------- 
 */

#include "interface.h"
#include "usb_def.h"
#include "usb_descriptors.h"
#include "gpio.h"

usb_ep_t 				endpoints[EP_COUNT];
static uint16_t 		lastaddr = LASTADDR_DEFAULT;
usb_dev_t 				USB_Dev;
static usb_LineCoding 	lineCoding = {9600, 0, 0, 8};

// EP0 buffers and supplement data
static config_pack_t 	setup_packet;
static uint8_t 			ep0databuf[EP0DATABUF_SIZE];
static uint8_t 			ep0dbuflen = 0;

// SET_LINE_CODING
void __attribute__((weak)) linecoding_handler(usb_LineCoding __attribute__((unused)) *lcd){
    //MSG("linecoding_handler()");
}

// SET_CONTROL_LINE_STATE
void __attribute__((weak)) clstate_handler(uint16_t __attribute__((unused)) val){
    //MSG("clstate_handler()");
}

// SEND_BREAK
void __attribute__((weak)) break_handler(){
    //MSG("break_handler()");
}

static void usb_ep0_sent(const uint8_t *buf, uint16_t size)
{
    if(setup_packet.wLength < size) size = setup_packet.wLength; // shortened request
    if(size < endpoints[0].txbufsz){
		usb_ep_write(0, buf, size);
        return;
    }
	
    while(size)
	{
        uint16_t l = size;
        if(l > endpoints[0].txbufsz) l = endpoints[0].txbufsz;
		usb_ep_write(0, buf, l);	// USB sent data
        buf += l;
        size -= l;
        uint8_t needzlp = (l == endpoints[0].txbufsz) ? 1 : 0;
        if(size || needzlp)
		{ 	// If send last data buffer
            uint16_t status = KEEP_DTOG(USB->EPnR[0]);
            // keep DTOGs, clear CTR_RX,TX, set TX VALID, leave stat_Rx
            USB->EPnR[0] = (status & ~(USB_EPnR_CTR_RX|USB_EPnR_CTR_TX|USB_EPnR_STAT_RX))
                            ^ USB_EPnR_STAT_TX;
            uint32_t ctr = 1000000;
            while(--ctr && (USB->ISTR & USB_ISTR_CTR) == 0);//{IWDG->KR = IWDG_REFRESH;};
            if((USB->ISTR & USB_ISTR_CTR) == 0){return;}
            if(needzlp) usb_ep_write(0, (uint8_t*)0, 0);
        }
    } 
}


static inline void get_descriptor()
{
    switch(setup_packet.wValue)
	{
        case DEVICE_DESCRIPTOR:
            usb_ep0_sent(USB_DeviceDescriptor, 18);
        break;
        case CONFIGURATION_DESCRIPTOR:
			usb_ep0_sent(USBD_CDC_CFG_DESCRIPTOR, sizeof(USBD_CDC_CFG_DESCRIPTOR));
        break;
        case STRING_LANG_DESCRIPTOR:
            usb_ep0_sent((const uint8_t *)&USB_StringLangDescriptor, STRING_LANG_DESCRIPTOR_SIZE_BYTE);
        break;
        case STRING_MAN_DESCRIPTOR:
            usb_ep0_sent((const uint8_t *)&USB_StringManufacturingDescriptor, USB_StringManufacturingDescriptor.bLength);
        break;
        case STRING_PROD_DESCRIPTOR:
            usb_ep0_sent((const uint8_t *)&USB_StringProdDescriptor, USB_StringProdDescriptor.bLength);
        break;
        case STRING_SN_DESCRIPTOR:
            usb_ep0_sent((const uint8_t *)&USB_StringSerialDescriptor, USB_StringSerialDescriptor.bLength);
        break;
        case DEVICE_QUALIFIER_DESCRIPTOR:
            usb_ep0_sent(USB_DeviceQualifierDescriptor, USB_DeviceQualifierDescriptor[0]);
        break;
        default:
        break;
    }
}


static uint8_t configuration = 0; // reply for GET_CONFIGURATION (==1 if configured)
void usb_ep_write(uint8_t number, const uint8_t *buf, uint16_t size)
{
    uint8_t i;
    if(size > endpoints[number].txbufsz) size = endpoints[number].txbufsz;
    uint16_t N2 = (size + 1) >> 1;
    // the buffer is 16-bit, so we should copy data as it would be uint16_t
    uint16_t *buf16 = (uint16_t *)buf;
    uint32_t *out = (uint32_t *)endpoints[number].tx_buf;
    for(i = 0; i < N2; ++i, ++out){
        *out = buf16[i];
    }
    USB_BTABLE->EP[number].USB_COUNT_TX = size;
}


static inline void std_d2h_req()
{
    uint16_t state = 0; // bus powered
    switch(setup_packet.bRequest)
	{
        case GET_DESCRIPTOR:
            get_descriptor();
        break;
        case GET_STATUS:
            usb_ep_write(0, (uint8_t *)&state, 2); // send status: Bus Powered
        break;
        case GET_CONFIGURATION:
            usb_ep_write(0, &configuration, 1);
        break;
        default:
        break;
    }
}



static inline void std_h2d_req()
{
    switch(setup_packet.bRequest)
	{
        case SET_ADDRESS:
            // new address will be assigned later - after acknowledgment or request to host
            USB_Dev.USB_Addr = setup_packet.wValue;
        break;
        case SET_CONFIGURATION:
            // Now device configured
            USB_Dev.USB_Status = USB_STATE_CONFIGURED;
            configuration = setup_packet.wValue;
        break;
        default:
        break;
    }
}



static void ep0_handler()
{
    uint8_t reqtype = setup_packet.bmRequestType & 0x7f;
    uint8_t dev2host = (setup_packet.bmRequestType & 0x80) ? 1 : 0;
    uint16_t epstatus = USB->EPnR[0];
    int rxflag = RX_FLAG(epstatus);
    if(rxflag && SETUP_FLAG(epstatus))
	{// If detected incoming (into device, out from host) SETUP transaction
        switch(reqtype){
            case STANDARD_DEVICE_REQUEST_TYPE: // standard device request
                if(dev2host)
				{	// IN transaction
                    std_d2h_req();
                }
				else
				{	//OUT transaction
                    std_h2d_req();
                    usb_ep_write(0, (uint8_t *)0, 0);
                }
            break;
            case STANDARD_ENDPOINT_REQUEST_TYPE: // standard endpoint request
                if(setup_packet.bRequest == CLEAR_FEATURE)
				{
                    usb_ep_write(0, (uint8_t *)0, 0);
                }
            break;
            case CONTROL_REQUEST_TYPE:
                switch(setup_packet.bRequest){
                    case GET_LINE_CODING:
                        usb_ep_write(0, (uint8_t*)&lineCoding, sizeof(lineCoding));
                    break;
                    case SET_LINE_CODING: // omit this for next stage, when data will come
                    break;
                    case SET_CONTROL_LINE_STATE:
                        clstate_handler(setup_packet.wValue);
                    break;
                    case SEND_BREAK:
                        break_handler();
                    break;
                    default:
                    break;
                }
                if(setup_packet.bRequest != GET_LINE_CODING) usb_ep_write(0, (uint8_t *)0, 0);
            break;
            default:
                usb_ep_write(0, (uint8_t *)0, 0);
        }
    }
	else if(rxflag)
	{ // got data over EP0 or host acknowledgment
        if(endpoints[0].rx_cnt){
            if(setup_packet.bRequest == SET_LINE_CODING){
                linecoding_handler((usb_LineCoding*)ep0databuf);
            }
        }
    }
	else if(TX_FLAG(epstatus))
	{ // package transmitted
        // now we can change address after enumeration
        if((USB->DADDR & USB_DADDR_ADD) != USB_Dev.USB_Addr){
            USB->DADDR = USB_DADDR_EF | USB_Dev.USB_Addr;
            // change state to ADDRESSED
            USB_Dev.USB_Status = USB_STATE_ADDRESSED;
}
		
    }
    epstatus = KEEP_DTOG(USB->EPnR[0]);
    if(rxflag) epstatus ^= USB_EPnR_STAT_TX; // start ZLP/data transmission
    else epstatus &= ~USB_EPnR_STAT_TX; // or leave unchanged
    // keep DTOGs, clear CTR_RX,TX, set RX VALID
    USB->EPnR[0] = (epstatus & ~(USB_EPnR_CTR_RX|USB_EPnR_CTR_TX)) ^ USB_EPnR_STAT_RX;
}

void usb_set_ep(uint8_t ep_num, uint8_t ep_type, uint16_t txsz, uint16_t rxsz, void (*func)(usb_ep_t ep))
{
	// Set endpoint type and endpoint address
    USB->EPnR[ep_num] = (ep_type << 9) | (ep_num & USB_EPnR_EA);
	// Initialize STAT_RX to VALID state by toggling both bits and STAT_TX to NAK state by toggling bit 1
    USB->EPnR[ep_num] ^= USB_EPnR_STAT_RX | USB_EPnR_STAT_TX_1;
    
	uint16_t countrx = 0;
    
	if(rxsz < 64) countrx = rxsz / 2;
    else countrx = 31 + rxsz / 32;
    
	//Set in EP[n]R register tx address
	uint8_t * i = &USB_BTABLE->EP[ep_num].USB_ADDR_TX;
	USB_BTABLE->EP[ep_num].USB_ADDR_TX = lastaddr;
    
	// Set endpoint tx data buffer pointer and size
	endpoints[ep_num].tx_buf = (uint16_t *)(USB_BTABLE_BASE + lastaddr*2);
    endpoints[ep_num].txbufsz = txsz;
    
	//Set in EP[n]R register rx address
	i = &USB_BTABLE->EP[ep_num].USB_COUNT_TX;
	USB_BTABLE->EP[ep_num].USB_COUNT_TX = 0;
	i = &USB_BTABLE->EP[ep_num].USB_ADDR_RX;
    USB_BTABLE->EP[ep_num].USB_ADDR_RX = lastaddr;
    
	endpoints[ep_num].rx_buf = (uint16_t *)(USB_BTABLE_BASE + lastaddr*2);
    
	lastaddr += txsz/2;
    
	// Set in in EP[n]R register
	i = &USB_BTABLE->EP[ep_num].USB_COUNT_RX;
	USB_BTABLE->EP[ep_num].USB_COUNT_RX = countrx << 10;
    
	// Set endpoint's callback
	endpoints[ep_num].func = func;
}


void USB_LP_CAN1_RX0_IRQHandler()
{
	// USB reset interrupt handler
    if(USB->ISTR & USB_ISTR_RESET)
	{
        // Reinit registers
        USB->CNTR = USB_CNTR_RESETM | USB_CNTR_CTRM | USB_CNTR_SUSPM | USB_CNTR_WKUPM | USB_CNTR_CTRM; 
        
		lastaddr = LASTADDR_DEFAULT;
        // clear address, leave only enable bit
        USB->DADDR = USB_DADDR_EF;
        // state is default - wait for enumeration
        USB_Dev.USB_Status = USB_STATE_DEFAULT;
        USB->ISTR = ~USB_ISTR_RESET;
        usb_set_ep(0, EP_CONTROL, 	USB_TXBUFSZ, USB_RXBUFSZ, ep0_handler);
		usb_set_ep(1, EP_INTERRUPT, USB_TXBUFSZ, USB_RXBUFSZ, 0);
		usb_set_ep(2, EP_BULK, 		USB_TXBUFSZ, USB_RXBUFSZ, 0);
		usb_set_ep(3, EP_BULK, 		USB_TXBUFSZ, USB_RXBUFSZ, 0);
		return;
    }
	
	//Correct transfer interrupt handler
	if(USB->ISTR & USB_ISTR_CTR)
	{
        // Get endpoint number
        uint8_t n = USB->ISTR & USB_ISTR_EPID;
        // Get endpoint status from EP[n] register
        uint16_t epstatus = USB->EPnR[n];
        // Get received bytes amount
        endpoints[n].rx_cnt = USB_BTABLE->EP[n].USB_COUNT_RX & 0x3FF; // low 10 bits is counter
        // Get transaction direction
        if(USB->ISTR & USB_ISTR_DIR)
		{// OUT interrupt - receive data, CTR_RX==1 (if CTR_TX == 1 - two pending transactions: receive following by transmit)
            if(n == 0) 
			{// control endpoint
                if(epstatus & USB_EPnR_SETUP)
				{ // If received setup packet -> read data to conf_packet
                    usb_read_ep(0, (uint16_t*)&setup_packet);
                    ep0dbuflen = 0;
                    // interrupt handler will be called later
                }
				else if(epstatus & USB_EPnR_CTR_RX)
				{ // If received data packet -> read received data to ep0databuf
                    ep0dbuflen = endpoints[0].rx_cnt;
                    usb_read_ep(0, (uint16_t*)&ep0databuf);
                }
            }
			else
			{
			
			// If OUT tocken on another endpoint (EP3 in this case)
			// Read first byte from RX buffer
			uint8_t b = *endpoints[n].rx_buf;
			
			// Write first byte to TX buffer of EP2
			//*((uint16_t*)USB_BTABLE->EP[2].USB_ADDR_TX) = b;
			endpoints[2].tx_buf[0] = b;
			// Set EP2 TX buffer size
			USB_BTABLE->EP[2].USB_COUNT_TX = 1;
			
			
			// If received symbol is '0' - turn off LED
			// If received symbol is '1' - turn on LED
			// If received symbol is '2' - invert LED state
			if( b == '0')
				SET_PIN(GPIOC, 13);
			else if( b == '1')
				RESET_PIN(GPIOC, 13);
			else if( b == '2')
				INVERT_PIN(GPIOC, 13);
			
			// Set EPn (EP3 in this case) status to VALID
			uint16_t val = KEEP_DTOG(USB->EPnR[n]);
			USB->EPnR[n] = (val & ~(USB_EPnR_CTR_RX|USB_EPnR_CTR_TX)) ^ USB_EPnR_STAT_RX;
			
			// Set EP2 status to VALID
			// If DTOG_TX is 1 set it to 0 (because I used single buffer for this data transfers)
			val = (USB->EPnR[2]);
			if(DTOG_FLAG(val))
				USB->EPnR[2] = (val & ~(USB_EPnR_CTR_RX|USB_EPnR_CTR_TX|USB_EPnR_STAT_TX)) ^ 0x0040;
			USB->EPnR[2] = (val & ~(USB_EPnR_CTR_RX|USB_EPnR_CTR_TX)) ^ USB_EPnR_STAT_TX;
			}
        }
		else
		{ // IN interrupt - transmit data, only CTR_TX == 1
            // enumeration end could be MSG (if EP0)
			if(n != 0)
			{
				uint16_t val = KEEP_DTOG(USB->EPnR[n]);
				val &=~ USB_EPnR_CTR_TX;
				if(TX_FLAG(val)) 
					val ^= USB_EPnR_STAT_TX;
				else 
					val &= ~USB_EPnR_STAT_TX;
				USB->EPnR[n] = (val & ~(USB_EPnR_CTR_RX|USB_EPnR_CTR_TX)) ^ USB_EPnR_STAT_TX;
			}
		}
        // Call endpoint handler
        if(endpoints[n].func) endpoints[n].func(endpoints[n]);
    }
	
	// Packet memory area over/underrun interrupt handler
	if (USB->ISTR & USB_ISTR_PMAOVR) 
	{
        USB->ISTR &= ~USB_ISTR_PMAOVR;
        // Handle PMAOVR status
        return;
    }
	
	// Suspend mode interrupt handler
    if (USB->ISTR & USB_ISTR_SUSP) 
	{
        USB->ISTR &= ~USB_ISTR_SUSP;
        if (USB->DADDR & 0x7f) 
		{
            USB->DADDR = 0;
            USB->CNTR &= ~ 0x800;
        }
        return;
    }
	
	// Error interrupt handler
    if (USB->ISTR & USB_ISTR_ERR) 
	{
        USB->ISTR &= ~USB_ISTR_ERR;
        // Handle Error
        return;
    }
	
	// Wakeup interrupt handler
    if (USB->ISTR & USB_ISTR_WKUP) 
	{
        USB->ISTR &= ~USB_ISTR_WKUP;
        // Handle Wakeup
        return;
    }
	
	// Start of frame interrupt handler
    if (USB->ISTR & USB_ISTR_SOF) 
	{
        USB->ISTR &= ~USB_ISTR_SOF;
        // Handle SOF
        return;
    }
	
	// Expected start of frame interrupt handler
    if (USB->ISTR & USB_ISTR_ESOF) 
	{
        USB->ISTR &= ~USB_ISTR_ESOF;
        // Handle ESOF
        return;
    }
    USB->ISTR = 0;
}

int usb_read_ep(uint8_t ep_num, uint16_t *buf)
{
    int sz = endpoints[ep_num].rx_cnt;
    if(!sz) return 0;
    endpoints[ep_num].rx_cnt = 0;
    int n = (sz + 1) >> 1;
    uint32_t *in = (uint32_t *)endpoints[ep_num].rx_buf;
    if(n)
	{
        for(int i = 0; i < n; ++i, ++in)
		{
            buf[i] = *(uint16_t*)in;
		}
    }
    return sz;
}

void _usb_init()
{
	//Init USB
	NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	
	USB->CNTR	= USB_CNTR_FRES;
	USB->BTABLE = 0;
	USB->DADDR	= 0;
	USB->ISTR	= 0;
	USB->CNTR	= USB_CNTR_RESETM;
	
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
