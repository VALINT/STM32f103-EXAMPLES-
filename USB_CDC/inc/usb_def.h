#ifndef _USB_DEF_H_
#define _USB_DEF_H_

#include "stm32f10x.h"

#define usb_int16(x) 			((x)&0xFF), (((x)>>8)&0xFF)

#define EP_COUNT          		8
#define LASTADDR_DEFAULT        (EP_COUNT * 8)

#define USB_BTABLE_SIZE         512
#define USB_BTABLE_BASE         0x40006000
#define USB_BASE                0x40005C00
#define USB                     ((USB_TypeDef *) USB_BASE)
#define USB_BTABLE              ((USB_BtableDef *)(USB_BTABLE_BASE))

#define USB_BUFSZ             	64
#define USB_TXBUFSZ             64
#define USB_RXBUFSZ             64

// USB requests
// Device requests
#define USB_REQUEST_GET_STATUS			0x00
#define USB_REQUEST_CLEAR_FETURE		0x01
#define USB_REQUEST_SET_FEATURE			0x03
#define USB_REQUEST_SET_ADDRESS			0x05
#define USB_REQUEST_GET_DESCRIPTOR		0x06
#define USB_REQUEST_SET_DESCRIPTOR		0x07
#define USB_REQUEST_GET_CONFIGURATION	0x08
#define USB_REQUEST_SET_CONFIGURATION	0x09
// Interface requests + 0x00, 0x01, 0x03
#define USB_REQUEST_GET_INTERFACE		0x0A
#define USB_REQUEST_SET_INTERFACE		0x11
// Endpoint request + 0x00, 0x01, 0x03
#define USB_REQUEST_SYNC_FRAME			0x12

#define USB_ISTR_EPID           0x0000000F
#define USB_FNR_LSOF_0          0x00000800
#define USB_FNR_lSOF_1          0x00001000
#define USB_LPMCSR_BESL_0       0x00000010
#define USB_LPMCSR_BESL_1       0x00000020
#define USB_LPMCSR_BESL_2       0x00000040
#define USB_LPMCSR_BESL_3       0x00000080
#define USB_EPnR_CTR_RX         0x00008000
#define USB_EPnR_DTOG_RX        0x00004000
#define USB_EPnR_STAT_RX        0x00003000
#define USB_EPnR_STAT_RX_0      0x00001000
#define USB_EPnR_STAT_RX_1      0x00002000
#define USB_EPnR_SETUP          0x00000800
#define USB_EPnR_EP_TYPE        0x00000600
#define USB_EPnR_EP_TYPE_0      0x00000200
#define USB_EPnR_EP_TYPE_1      0x00000400
#define USB_EPnR_EP_KIND        0x00000100
#define USB_EPnR_CTR_TX         0x00000080
#define USB_EPnR_DTOG_TX        0x00000040
#define USB_EPnR_STAT_TX        0x00000030
#define USB_EPnR_STAT_TX_0      0x00000010
#define USB_EPnR_STAT_TX_1      0x00000020
#define USB_EPnR_EA             0x0000000F
#define USB_COUNTn_RX_BLSIZE    0x00008000
#define USB_COUNTn_NUM_BLOCK    0x00007C00
#define USB_COUNTn_RX           0x0000003F

// EPxREG: EndPoint Registers Bit Definitions 
#define EP_CTR_RX 				0x8000 // Correct RX Transfer
#define EP_DTOG_RX 				0x4000 // RX Data Toggle
#define EP_STAT_RX 				0x3000 // RX Status
#define EP_SETUP 				0x0800 // EndPoint Setup
#define EP_TYPE 				0x0600 // EndPoint Type
#define EP_KIND 				0x0100 // EndPoint Kind
#define EP_CTR_TX 				0x0080 // Correct TX Transfer
#define EP_DTOG_TX 				0x0040 // TX Data Toggle
#define EP_STAT_TX 				0x0030 // TX Status
#define EP_EA 					0x000F // EndPoint Address
#define EP_MASK (EP_CTR_RX | EP_SETUP | EP_TYPE | EP_KIND | EP_CTR_TX | EP_EA)

/* EP_STAT_TX: TX Status */
#define TX_DISABLE 0x0000 /* Disabled */
#define TX_STALL 0x0010   /* Stalled */
#define TX_NAK 0x0020     /* NAKed */
#define TX_VALID 0x0030   /* Valid */

/* EP_TYPE: EndPoint Types */
#define EP_BULK                    0x00
#define EP_CONTROL                 0x01
#define EP_ISO                     0x02
#define EP_INTERRUPT               0x03

/* USB Descriptor Types */
#define USB_DEVICE_DESC_TYPE 1
#define USB_CFG_DESC_TYPE 2
#define USB_STR_DESC_TYPE 3
#define USB_IFACE_DESC_TYPE 4
#define USB_EP_DESC_TYPE 5
#define USB_DEVICE_QR_DESC_TYPE 6
#define USB_OSPEED_CFG_DESC_TYPE 7
#define USB_IFACE_PWR_DESC_TYPE 8

// Max EP amount (EP0 + other used)

#define STANDARD_DEVICE_REQUEST_TYPE    0
#define STANDARD_ENDPOINT_REQUEST_TYPE  2
#define VENDOR_REQUEST_TYPE             0x40
#define CONTROL_REQUEST_TYPE            0x21
// bRequest, standard; for bmRequestType == 0x80
#define GET_STATUS                      0x00
#define GET_DESCRIPTOR                  0x06
#define GET_CONFIGURATION               0x08
// for bmRequestType == 0
#define CLEAR_FEATURE                   0x01
#define SET_FEATURE                     0x03    // unused
#define SET_ADDRESS                     0x05
#define SET_DESCRIPTOR                  0x07    // unused
#define SET_CONFIGURATION               0x09
// for bmRequestType == 0x81, 1 or 0xB2
#define GET_INTERFACE                   0x0A    // unused
#define SET_INTERFACE                   0x0B    // unused
#define SYNC_FRAME                      0x0C    // unused
#define VENDOR_REQUEST                  0x01    // unused

#define RX_FLAG(epstat)                 (epstat & USB_EPnR_CTR_RX)
#define TX_FLAG(epstat)                 (epstat & USB_EPnR_CTR_TX)
#define DTOG_FLAG(epstat)				(epstat & USB_EPnR_DTOG_TX)
#define SETUP_FLAG(epstat)              (epstat & USB_EPnR_SETUP)

// keep all DTOGs and STATs
#define KEEP_DTOG_STAT(EPnR)            (EPnR & ~(USB_EPnR_STAT_RX|USB_EPnR_STAT_TX|USB_EPnR_DTOG_RX|USB_EPnR_DTOG_TX))
#define KEEP_DTOG(EPnR)                 (EPnR & ~(USB_EPnR_DTOG_RX|USB_EPnR_DTOG_TX))

// Class-Specific Control Requests
#define SEND_ENCAPSULATED_COMMAND       0x00    // unused
#define GET_ENCAPSULATED_RESPONSE       0x01    // unused
#define SET_COMM_FEATURE                0x02    // unused
#define GET_COMM_FEATURE                0x03    // unused
#define CLEAR_COMM_FEATURE              0x04    // unused
#define SET_LINE_CODING                 0x20
#define GET_LINE_CODING                 0x21
#define SET_CONTROL_LINE_STATE          0x22
#define SEND_BREAK                      0x23

// control line states
#define CONTROL_DTR                     0x01
#define CONTROL_RTS                     0x02

// wValue
#define DEVICE_DESCRIPTOR               0x100
#define CONFIGURATION_DESCRIPTOR        0x200
#define STRING_LANG_DESCRIPTOR          0x300
#define STRING_MAN_DESCRIPTOR           0x301
#define STRING_PROD_DESCRIPTOR          0x302
#define STRING_SN_DESCRIPTOR            0x303
#define DEVICE_QUALIFIER_DESCRIPTOR     0x600

#define EP0DATABUF_SIZE                 (64)

#define KEEP_DTOG_STAT(EPnR)            (EPnR & ~(USB_EPnR_STAT_RX|USB_EPnR_STAT_TX|USB_EPnR_DTOG_RX|USB_EPnR_DTOG_TX))
#define KEEP_DTOG(EPnR)                 (EPnR & ~(USB_EPnR_DTOG_RX|USB_EPnR_DTOG_TX))

#define _USB_LANG_ID_(lng_id)     \
    \
const struct wLANGID \
{         \
        uint8_t  bLength;         \
        uint8_t  bDescriptorType; \
        uint16_t bString[1];      \
    \
} \
wLANGID __attribute__((used, section("usb_string"))) = {0x04, 0x03, lng_id};

#define LANG_US (uint16_t)0x0409

#define USB_STRING(name, str)                  \
static const struct name \
{                          \
        uint8_t  bLength;                       \
        uint8_t  bDescriptorType;               \
        uint16_t bString[(sizeof(str) - 2) / 2]; \
    \
} \
name = {sizeof(name), 0x03, str}

#define USB_LANG_ID(name, lng_id)     \
    \
static const struct name \
{         \
        uint8_t  bLength;         \
        uint8_t  bDescriptorType; \
        uint16_t bString;         \
    \
} \
name = {0x04, 0x03, lng_id}
#define STRING_LANG_DESCRIPTOR_SIZE_BYTE    (4)

typedef struct {
    __IO uint32_t EPnR[EP_COUNT];
    __IO uint32_t RESERVED[EP_COUNT];
    __IO uint32_t CNTR;
    __IO uint32_t ISTR;
    __IO uint32_t FNR;
    __IO uint32_t DADDR;
    __IO uint32_t BTABLE;
} USB_TypeDef;

typedef struct{
    __IO uint32_t USB_ADDR_TX;
    __IO uint32_t USB_COUNT_TX;
    __IO uint32_t USB_ADDR_RX;
    __IO uint32_t USB_COUNT_RX;
} usb_ep_data_t;

typedef struct{
    volatile usb_ep_data_t EP[EP_COUNT];
} USB_BtableDef;

// EP0 configuration packet
typedef struct {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} config_pack_t;

typedef struct usb_request_type_t
{
    uint8_t Recipient : 5;
    uint8_t Type : 2;
    uint8_t Dir : 1;
} usb_request_type_t;

// endpoints state
typedef struct usb_ep_t{
    uint16_t *tx_buf;           // transmission buffer address
    uint16_t txbufsz;           // transmission buffer size
    uint16_t *rx_buf;           // reception buffer address
    void (*func)();             // endpoint action function
    unsigned rx_cnt  : 10;      // received data counter
} usb_ep_t;

typedef struct
{
    uint8_t L : 8;
    uint8_t H : 8;
} usb_wbyte_t;

typedef struct usb_setup_packet_t
{
    usb_request_type_t 	bmRequestType;
    uint8_t            	bRequest;
    usb_wbyte_t			wValue;
    usb_wbyte_t       	wIndex;
    uint8_t            	wLength;
} usb_setup_packet_t;

typedef struct {
    uint8_t  USB_Status;
    uint16_t USB_Addr;
}usb_dev_t;

typedef enum{
    USB_STATE_DEFAULT,
    USB_STATE_ADDRESSED,
    USB_STATE_CONFIGURED,
    USB_STATE_CONNECTED
} USB_state;

typedef struct {
    uint32_t dwDTERate;
    uint8_t bCharFormat;
    #define USB_CDC_1_STOP_BITS   0
    #define USB_CDC_1_5_STOP_BITS 1
    #define USB_CDC_2_STOP_BITS   2
    uint8_t bParityType;
    #define USB_CDC_NO_PARITY     0
    #define USB_CDC_ODD_PARITY    1
    #define USB_CDC_EVEN_PARITY   2
    #define USB_CDC_MARK_PARITY   3
    #define USB_CDC_SPACE_PARITY  4
    uint8_t bDataBits;
} __attribute__ ((packed)) usb_LineCoding;

#endif
