#ifndef _USB_DESCRIPTOR_H_
#define _USB_DESCRIPTOR_H_

#include "stdint.h"
#include "usb_def.h"

typedef struct USB_DEVICE_DESCRIPTOR{
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bcdUSB;
	uint8_t		bDeviceClass;
	uint8_t		bDeviseSubClass;
	uint8_t		bDeviceProtocol;
	uint8_t		bMaxPacketSize;
	uint16_t	idVendor;
	uint16_t	idProduct;
	uint16_t	bcdDevice;
	uint8_t		iManufacturer;
	uint8_t		iProduct;
	uint8_t		iSerialNumber;
	uint8_t		bNumConfigurations;
}USB_DEVICE_DESCRIPTOR;

typedef struct _USB_DEVICE_QUALIFIER_DESCRIPTOR {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bcdUSB;
	uint8_t		bDeviceClass;
	uint8_t		bDeviceSubClass;
	uint8_t		bDeviceProtocol;
	uint8_t		bMaxPacketSize0;
	uint8_t		bNumConfigurations;
	uint8_t		bReserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR;

typedef struct USB_CONFIGURATION_DECRIPTOR{
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	wTotalLength;
	uint8_t		bNumInterfaces;
	uint8_t		bConfigurationValue;
	uint8_t		iConfiguration;
	uint8_t		bmAttributes;
	uint8_t		bMaxPower;
}USB_CONFIGURATION_DECRIPTOR;

typedef struct USB_INTERFACE_DECRIPTOR{
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint8_t		bInterfaceNumber;
	uint8_t		bAlternateSetting;
	uint8_t		bNumEndpoints;
	uint8_t		bInterfaceClass;
	uint8_t		bInterfaceSubClass;
	uint8_t		bInterfaceProtocol;
	uint8_t		iInterface;
}USB_INTERFACE_DECRIPTOR;

typedef struct USB_ENDPOINT_DECRIPTOR{
	uint8_t		bLength;	
	uint8_t		bDescriptorType;	
	uint8_t		bEndpointAddress;
	uint8_t		bmAttributes;
	uint16_t	wMaxPacketSize;
	uint8_t		bInterval;
}USB_ENDPOINT_DECRIPTOR;

typedef struct USB_STRING_DESCRIPTOR{
    uint8_t bLength;
    uint8_t bDescriptorType;
} USB_STRING_DESCRIPTOR;


USB_LANG_ID(USB_StringLangDescriptor, LANG_US);
USB_STRING(USB_StringSerialDescriptor, u"dev-prototype");
USB_STRING(USB_StringManufacturingDescriptor, u"VAL");
USB_STRING(USB_StringProdDescriptor, u"USB - Saturn_mk3");

#define bcdUSB_L        	0x00
#define bcdUSB_H        	0x02
#define bDeviceClass    	2
#define bDeviceSubClass 	0
#define bDeviceProtocol 	0
#define bNumConfigurations 	1

static const uint8_t USB_DeviceDescriptor[] = {
        18,     			// bLength
        0x01,   			// bDescriptorType - Device descriptor
        bcdUSB_L,   		// bcdUSB_L - 2.0
        bcdUSB_H,   		// bcdUSB_H
        bDeviceClass,   	// bDeviceClass - USB_COMM
        bDeviceSubClass,   	// bDeviceSubClass
        bDeviceProtocol,   	// bDeviceProtocol
        USB_BUFSZ,   		// bMaxPacketSize
		// 0483:5740 (VID:PID) - stm32 VCP
		0x83, 				// VID_L
		0x04, 				// VID_H
		0x40, 				// PID_L
		0x57,				// PID_H
        0x01,   			// bcdDevice_Ver_L
        0x00,   			// bcdDevice_Ver_H
        0x01,   			// iManufacturer
        0x02,   			// iProduct
        0x03,   			// iSerialNumber
        bNumConfigurations	// bNumConfigurations
};

static const uint8_t USB_DeviceQualifierDescriptor[] = {
        10,     			//bLength
        0x06,   			// bDescriptorType - Device qualifier
        bcdUSB_L,   		// bcdUSB_L
        bcdUSB_H,   		// bcdUSB_H
        bDeviceClass,   	// bDeviceClass
        bDeviceSubClass,   	// bDeviceSubClass
        bDeviceProtocol,   	// bDeviceProtocol
        USB_BUFSZ,   		// bMaxPacketSize0
        bNumConfigurations, // bNumConfigurations
        0x00    			// Reserved
};
const uint8_t USBD_CDC_CFG_DESCRIPTOR[] =
    {
        /*Configuration Descriptor*/
        0x09, /* bLength: Configuration Descriptor size */
        0x02, /* bDescriptorType: Configuration */
        67,   /* wTotalLength:no of returned bytes */
        0x00,
        0x02, /* bNumInterfaces: 2 interface */
        0x01, /* bConfigurationValue: Configuration value */
        0x00, /* iConfiguration: Index of string descriptor describing the configuration */
        0x80, /* bmAttributes - Bus powered */
        0x32, /* MaxPower 100 mA */

        /*---------------------------------------------------------------------------*/

        /*Interface Descriptor */
        0x09, /* bLength: Interface Descriptor size */
        0x04, /* bDescriptorType: Interface */
        0x00, /* bInterfaceNumber: Number of Interface */
        0x00, /* bAlternateSetting: Alternate setting */
        0x01, /* bNumEndpoints: One endpoints used */
        0x02, /* bInterfaceClass: Communication Interface Class */
        0x02, /* bInterfaceSubClass: Abstract Control Model */
        //0x01, /* bInterfaceProtocol: Common AT commands */
		0x00, /* bInterfaceProtocol: No protocol */
        0x00, /* iInterface: */

        /*Header Functional Descriptor*/
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,

        /*Call Management Functional Descriptor*/
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x01, /* bDescriptorSubtype: Call Management Func Desc */
        0x00, /* bmCapabilities: D0+D1 */
        0x01, /* bDataInterface: 1 */

        /*ACM Functional Descriptor*/
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x02, /* bmCapabilities */

        /*Union Functional Descriptor*/
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x06, /* bDescriptorSubtype: Union func desc */
        0x00, /* bMasterInterface: Communication class interface */
        0x01, /* bSlaveInterface0: Data Class Interface */

        /*Endpoint 2 Descriptor*/
        0x07, /* bLength: Endpoint Descriptor size */
        0x05, /* bDescriptorType: Endpoint */
        0x81, /* bEndpointAddress IN1 */
        0x03, /* bmAttributes: Interrupt */
        0x08, /* wMaxPacketSize LO: */
        0x00, /* wMaxPacketSize HI: */
        0x10, /* bInterval: */
        /*---------------------------------------------------------------------------*/

        /*Data class interface descriptor*/
        0x09, /* bLength: Endpoint Descriptor size */
        0x04, /* bDescriptorType: */
        0x01, /* bInterfaceNumber: Number of Interface */
        0x00, /* bAlternateSetting: Alternate setting */
        0x02, /* bNumEndpoints: Two endpoints used */
        0x0A, /* bInterfaceClass: CDC */
        0x02, /* bInterfaceSubClass: */
        0x00, /* bInterfaceProtocol: */
        0x00, /* iInterface: */

        /*Endpoint IN2 Descriptor*/
        0x07, /* bLength: Endpoint Descriptor size */
        0x05, /* bDescriptorType: Endpoint */
        0x82, /* bEndpointAddress IN2 */
        0x02, /* bmAttributes: Bulk */
        64,   /* wMaxPacketSize: */
        0x00,
        0x00, /* bInterval: ignore for Bulk transfer */

        /*Endpoint OUT3 Descriptor*/
        0x07, /* bLength: Endpoint Descriptor size */
        0x05, /* bDescriptorType: Endpoint */
        0x03, /* bEndpointAddress */
        0x02, /* bmAttributes: Bulk */
        64,   /* wMaxPacketSize: */
        0,
        0x00 /* bInterval: ignore for Bulk transfer */
};

#endif
