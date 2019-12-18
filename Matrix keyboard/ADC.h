/*
 * ADC.h
 *
 * Created: 13.10.2019 13:35:35
 *  Author: Din
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 2

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (1<<ADLAR))

volatile uint8_t adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];

void ADC_Init();

#endif /* ADC_H_ */