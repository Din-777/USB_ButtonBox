/*
 * ADC.c
 *
 * Created: 13.10.2019 13:35:51
 *  Author: Din
 */ 

#include "ADC.h"

void ADC_Init()
{
	// ADC initialization
	// ADC Clock frequency: 750,000 kHz
	// ADC Voltage Reference: AREF pin
	// Only the 8 most significant bits of
	// the AD conversion result are used
	ADMUX=ADC_VREF_TYPE;
	ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
	SFIOR=(0<<ACME);
}

void ADC_Scan(uint8_t out[])
{	
	ADMUX = 0 | ADC_VREF_TYPE;			// Select ADC input
	asm("NOP"); //asm("NOP"); asm("NOP");	// Delay needed for the stabilization of the ADC input voltage
	
	ADCSRA |= (1<<ADSC);				// Start the AD conversion	

	while ((ADCSRA & (1<<ADIF) ) == 0);	// Wait for the AD conversion to complete
	
	out[0] = ADCH;
	
	ADMUX = 1 | ADC_VREF_TYPE;			// Select ADC input
	asm("NOP"); //asm("NOP"); asm("NOP");	// Delay needed for the stabilization of the ADC input voltage
	
	ADCSRA |= (1<<ADSC);				// Start the AD conversion

	while ((ADCSRA & (1<<ADIF)) == 0);	// Wait for the AD conversion to complete
	
	out[1] = ADCH;
	
	ADMUX = 2 | ADC_VREF_TYPE;			// Select ADC input
	asm("NOP"); //asm("NOP"); asm("NOP");	// Delay needed for the stabilization of the ADC input voltage
	
	ADCSRA |= (1<<ADSC);				// Start the AD conversion

	while ((ADCSRA & (1<<ADIF)) == 0);	// Wait for the AD conversion to complete
	
	out[2] = ADCH;
}