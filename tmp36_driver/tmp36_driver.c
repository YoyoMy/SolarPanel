
/*
 * tmp36_driver.c
 *
 * Created: 22/03/2023 09.36.11
 *  Authors : haled & yoana
 */ 

#include "avr/io.h"
#include <stdio.h>
#include "avr/interrupt.h"
#define F_CPU 16000000L
// A static variable to store the ADC measurement
static uint16_t sample = 0;
// A function pointer to the callback function
static void (*callback)() = NULL;

void tmp36_init(void (*cb)()){
	//Set VOUT as input
	DDRK &= ~_BV(PK7);
	
	ADMUX |= _BV(REFS0) | _BV(MUX0) | _BV(MUX1) | _BV(MUX2);
	ADCSRB |= _BV(MUX5) | _BV(ADTS2) | _BV(ADTS0);
	
	// Enable the ADC, enable auto-triggering, enable interrupts, and set the ADC clock prescaler to 128
	ADCSRA |= _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	
	//Set ShutDown as 1 (enabled)
	DDRG |= _BV(PG0);
	PORTG |= _BV(PG0);

	callback = cb;
}

void tmp36_measure(){
	//Start ADC converter
	 ADCSRA |= _BV(ADSC);
}

uint16_t get_measurement(){
	return sample;
}

ISR(ADC_vect){
	sample = ADC;
	TIFR1 |= _BV(OCF1B);
	if (callback != NULL)
		callback();
}

