#define F_CPU 12000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "usbdrv\usbdrv.h"	
#include "encoder.h"

static uchar reportBuffer[3] = {0,0,0} ;
static uchar idleRate;           /* in 4 ms units */

PROGMEM const char usbHidReportDescriptor[25] PROGMEM = { /* USB report descriptor */
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
};



/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    usbMsgPtr = reportBuffer;
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
			return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
			idleRate = rq->wValue.bytes[1];
        
        }else if(rq->bRequest == USBRQ_HID_GET_PROTOCOL){
        
        }else if(rq->bRequest == USBRQ_HID_SET_PROTOCOL){
        }
		
    }else{
        /* no vendor specific requests implemented */
    }
	return 0;
}
/* ------------------------------------------------------------------------- */

int main(void)
{
    uchar encstate;
    uchar Btnstate = 0;
    uchar LastBtnstate = 0;
	uchar LastKeyPress = 0;
	uchar KeyPressed = 0;
	ENC_InitEncoder();
    usbInit();
    usbDeviceDisconnect();  
    uchar i = 0;
    while(--i){             
        _delay_ms(1);

    }
    
    usbDeviceConnect();     

    sei();                  
	reportBuffer[0] = 1;  // ReportID = 1
	reportBuffer[2] = 0;
	for(;;){                
      usbPoll();          
      ENC_PollEncoder();
	  
      ///////////////////////////////////////////////
      
      KeyPressed = 0;
	  encstate = ENC_GetStateEncoder();
      if (LEFT_SPIN == encstate)
      {
         KeyPressed = 0xea;
      }
      else if (RIGHT_SPIN == encstate)
      {
         KeyPressed = 0xe9;
      }
	  Btnstate = ENC_GetBtnState();
	  if (Btnstate != LastBtnstate)
	  {
	    if (Btnstate != 1) KeyPressed = 0xe2;				
		LastBtnstate = Btnstate;
	  }	
        
	  if(LastKeyPress != KeyPressed){
		 if (usbInterruptIsReady()){
			LastKeyPress = KeyPressed;
			reportBuffer[1] = KeyPressed;
			/* use last key and not current key status in order to avoid lost
             changes in key status. */
			usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
		 }	
		/* This block sets the the number of additional keypress a volume key. 
		This increases the rate of change of volume of from 2 to 50 times
		The number of additional keypress sets by the variable AdditionalKeyPress. 	*/
		uchar AdditionalKeyPress = 0;
		while(AdditionalKeyPress--){ 
			if ((KeyPressed == 0xea)||(KeyPressed == 0xe9)){
				while (!(usbInterruptIsReady())){}
				usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
			}
		}//End of block		
      }
	}
    return 0;
}