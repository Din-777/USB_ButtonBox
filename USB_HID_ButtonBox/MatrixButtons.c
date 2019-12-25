/*
 * MatrixKeyboard.c
 *
 * Created: 07.10.2019 16:08:31
 *  Author: User
 */ 

#include "MatrixButtons.h"
	
void MatrixButtons_Init()
{
	ROL_DDR  |= (1 << ROL_PIN_1) | (1 << ROL_PIN_2) | (1 << ROL_PIN_3);
	ROL_PORT |= (1 << ROL_PIN_1) | (1 << ROL_PIN_2) | (1 << ROL_PIN_3);
	
	COL_DDR  &= ~((1 << COL_PIN_A) | (1 << COL_PIN_B) | (1 << COL_PIN_C) | (1 << COL_PIN_D)); 
	COL_PORT |=   (1 << COL_PIN_A) | (1 << COL_PIN_B) | ( 1<< COL_PIN_C) | (1 << COL_PIN_D);
	
	BT_DDR  &= ~((1 << BT_PIN_1) | (1 << BT_PIN_3));
	BT_PORT |=   (1 << BT_PIN_1) | (1 << BT_PIN_3);	
	
	ADC_Init();
}

volatile uint8_t  counter		= 0;
volatile uint32_t prev_state	= 0;
volatile uint8_t  prev_state_0	= 0;

 uint8_t adcData[3] = {0,0,0};

void MatrixButtons_Scan(uint8_t out[])
{
	unsigned char curr_state[3] = {0, 0, 0};
	
	curr_state[0]  =  (BT_PIN & (1 << BT_PIN_1)) >> BT_PIN_1;
	curr_state[0] |= ((BT_PIN & (1 << BT_PIN_3)) >> BT_PIN_3) << 2;
	
	ROL_PORT &= ~(1<<ROL_PIN_1);
	asm("NOP"); asm("NOP"); asm("NOP");
	curr_state[0] |= ((COL_PIN & 0b00001111) << 4);
	ROL_PORT |= (1<<ROL_PIN_1);
	asm("NOP"); asm("NOP"); asm("NOP");
	
	ROL_PORT &= ~(1<<ROL_PIN_2);
	asm("NOP"); asm("NOP"); asm("NOP");
	curr_state[1] = (COL_PIN & 0b00001111);
	ROL_PORT |= (1<<ROL_PIN_2);
	asm("NOP"); asm("NOP"); asm("NOP");
	
	ROL_PORT &= ~(1<<ROL_PIN_3);
	asm("NOP"); asm("NOP"); asm("NOP");
	curr_state[1] |= ((COL_PIN & 0b00001111) << 4);
	ROL_PORT |= (1<<ROL_PIN_3);
	asm("NOP"); asm("NOP"); asm("NOP");

	curr_state[0] = ~(curr_state[0] | 0b00001010);
	curr_state[1] = ~curr_state[1];
	
	ADC_Scan(adcData);
	
	if(adcData[0] < 50)  curr_state[2] |= (1 << 4);
	if(adcData[0] > 200) curr_state[2] |= (1 << 5);
	if(adcData[1] < 50)  curr_state[2] |= (1 << 2);
	if(adcData[1] > 200) curr_state[2] |= (1 << 3);
	if(adcData[2] < 50)  curr_state[2] |= (1 << 0);
	if(adcData[2] > 200) curr_state[2] |= (1 << 1);
	
	//--------------------    тумблер    ---------------------
	
	//-----------------------------------------------
	
	
	//--------------------	 удержание    ---------------------------------
	if( prev_state == ( curr_state[0] | (curr_state[1] << 7) | (curr_state[2] << 15) ) )
	{
		counter += 1;
		prev_state = curr_state[0] | (curr_state[1] << 7) | (curr_state[2] << 15);
		if((counter > 30) && (counter & 1) )
		{
			curr_state[0] &= 0b00000111;
			curr_state[1] = 0;
			curr_state[2] = 0;
			counter = 33;
		}
	}
	else
	{
		counter = 0;		
		prev_state = curr_state[0] | (curr_state[1] << 7) | (curr_state[2] << 15);		
	}		
	//----------------------------------------------------------------------
		
	out[0] = curr_state[0];
	out[1] = curr_state[1];
	out[2] = curr_state[2];
}
