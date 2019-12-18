/*
 * ADC.h
 *
 * Created: 13.10.2019 13:35:35
 *  Author: Din
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

#define FIRST_ADC_INPUT		0
#define LAST_ADC_INPUT		2

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (1<<ADLAR))

void ADC_Init();
void ADC_Scan(uint8_t out[]);

#endif /* ADC_H_ */