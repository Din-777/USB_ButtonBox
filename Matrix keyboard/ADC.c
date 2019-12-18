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
	// ADC Clock frequency: 375,000 kHz
	// ADC Voltage Reference: AREF pin
	// Only the 8 most significant bits of
	// the AD conversion result are used
	ADMUX=FIRST_ADC_INPUT | ADC_VREF_TYPE;
	ADCSRA=(1<<ADEN) | (1<<ADSC) | (0<<ADFR) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
	SFIOR=(0<<ACME);
}

// ADC interrupt service routine
// with auto input scanning
ISR(ADC_vect)
{
	static unsigned char input_index=0;
	// Read the 8 most significant bits
	// of the AD conversion result
	adc_data[input_index]=ADCH;
	
	// Select next ADC input
	if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
		input_index=0;
		
	ADMUX=(FIRST_ADC_INPUT | ADC_VREF_TYPE)+input_index;
	// Delay needed for the stabilization of the ADC input voltage
	_delay_us(10);
	
	// Start the AD conversion
	ADCSRA|=(1<<ADSC);
}