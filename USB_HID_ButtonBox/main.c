/*
 * USB_HID_ButtonBox.c
 *
 * Created: 11.10.2019 16:44:11
 * Author : User
 */ 

#define F_CPU 12000000UL
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "usbdrv\usbdrv.h"
#include "MatrixButtons.h"

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x18,                    //   USAGE_MAXIMUM (Button 24)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x18,                    //   REPORT_COUNT (24)
    0x55, 0x00,                    //   UNIT_EXPONENT (0)
    0x65, 0x00,                    //   UNIT (None)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

/*static uchar reportBuffer[3] = {0,0,0} ;
static uchar idleRate;           // in 4 ms units 

PROGMEM const char usbHidReportDescriptor[25] PROGMEM = { // USB report descriptor 
	0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
	0x09, 0x01,                    // USAGE (Consumer Control)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, 0x01,                    //   REPORT_ID (1)
	0x19, 0x00,                    //   USAGE_MINIMUM (Unassigned)
	0x2a, 0x3c, 0x02,              //   USAGE_MAXIMUM (AC Format)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0x3c, 0x02,              //   LOGICAL_MAXIMUM (572)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x75, 0x10,                    //   REPORT_SIZE (16)
	0x81, 0x00,                    //   INPUT (Data,Var,Abs)
	0xc0                           // END_COLLECTION
};*/

static uchar reportBuffer[3] = {0,0,0};
static uchar idleRate;           /* in 4 ms units */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;

	usbMsgPtr = reportBuffer;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{    
		/* class request type */
		if(rq->bRequest == USBRQ_HID_GET_REPORT)
		{  
			/* wValue: ReportType (highbyte), ReportID (lowbyte) */
			/* we only have one report type, so don't look at wValue */
			return sizeof(reportBuffer);
		}
		else if(rq->bRequest == USBRQ_HID_GET_IDLE)
		{
			usbMsgPtr = &idleRate;
			return 1;
		}
		else if(rq->bRequest == USBRQ_HID_SET_IDLE)
		{
			idleRate = rq->wValue.bytes[1];
		}
		else if(rq->bRequest == USBRQ_HID_GET_PROTOCOL)
		{			
		}
		else if(rq->bRequest == USBRQ_HID_SET_PROTOCOL)
		{
		}		
	}
	else
	{
		/* no vendor specific requests implemented */
	}
	return 0;
}

int main(void)
{
	usbInit();
	MatrixButtons_Init();
	usbDeviceDisconnect();
	
	uchar i = 0;
	while(--i){
		_delay_ms(1);
	}
	
	usbDeviceConnect();

	sei();
	
	for(;;)
	{
		usbPoll();	
		
		if (usbInterruptIsReady())
		{
			MatrixButtons_Scan(reportBuffer);		
			usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
		}
	}

	return 0;
}
