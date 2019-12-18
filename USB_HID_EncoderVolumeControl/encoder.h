//Fuses LOW FUSE BYTE: 0xCF, HIGH FUSE BYTE: 0x18, LOCKOPT BYTE: 0x3F. 

#ifndef	encoder_h
#define	encoder_h
#include <avr/io.h>
//_________________________________________
//encoder port & pins
#define PORT_Enc 	PORTB 	
#define PIN_Enc 	PINB
#define DDR_Enc 	DDRB
#define Pin1_Enc 	4
#define Pin2_Enc 	5
#define Btn_Enc 	2
//______________________
#define RIGHT_SPIN 0x01 
#define LEFT_SPIN 0xff

void ENC_InitEncoder(void);
void ENC_PollEncoder(void);
unsigned char ENC_GetStateEncoder(void);
unsigned char ENC_GetBtnState(void);
#endif  //encoder_h
