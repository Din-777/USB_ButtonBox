/*
 * Matrix keyboard.c
 *
 * Created: 07.10.2019 16:02:20
 * Author : User
 */ 

#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "MatrixButtons.h"
#include "ADC.h"

static unsigned char reportBuffer[3] = {0,0,0};

int main(void)
{
	ADC_Init();
	sei();
	MatrixButtons_Init();
	reportBuffer[0] = 1;
	
    while (1) 
    {
		 MatrixButtons_Scan(reportBuffer);
		_delay_ms(1);
    }
}