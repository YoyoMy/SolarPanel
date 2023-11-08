/*
 * timer_driver.c
 *
 * Created: 22/03/2023 10.36.11
 *  Authors : haled & yoana
 */ 

#include "avr/io.h"
#include "avr/interrupt.h"
#include <stdio.h>
#include "timer_driver.h"
#include "spi_driver.h"

#define F_CPU 16000000L

static void (*callback)() = NULL;

void timer1_start(){
    //Wave form generateion mode - CTC / Setup Timer 1 pre-scaler to clk/256
	TCCR1B |= _BV(CS12) | _BV(WGM12);
    
    //OCR1B = (F_CPU / (2 * pre-scaler[256] * desired frequency(1Hz])) -1
	OCR1B = (F_CPU / (2*256*1))-1;
    //Setting the top value
    OCR1A = (F_CPU / (2*256*1))-1;
    
}

void timer0_start(void (*cb)()) {
	if(cb != NULL) callback = cb;
	//Wave form generateion mode - CTC / Setup Timer 0 pre-scaler to clk/64
    TCCR0A |= _BV(WGM01);
	TCCR0B |= _BV(CS02) | _BV(CS00);

	//Enable timer 4 interrupt so we can use the interrupt service
	TIMSK0 |= _BV(OCIE0A);
    
    //OCR4A = (F_CPU / (2 * pre-scaler[64] * desired frequency(1000Hz])) -1
    OCR0A = (F_CPU / (1024*240));
}

ISR(TIMER0_COMPA_vect) {
	if(callback != NULL) callback();
}
