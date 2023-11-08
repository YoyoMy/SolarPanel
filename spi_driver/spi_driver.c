/*
 * spi_driver.c
 *
 * Created: 14/04/2023 09.01.11
 *  Authors : haled & yoana
 */

#include <stdio.h>
#include "avr/io.h"
#include "avr/interrupt.h"
#define F_CPU 16000000L

void init_spi(){
	/* Set MOSI and SCK output, all others input */
	DDRB = _BV(PB2)| _BV(PB1) | _BV(PB0);

	/* Enable SPI, Master, set clock rate fck/2	 */
	SPCR = _BV(SPE) | _BV(DORD) | _BV(MSTR) | _BV(CPOL) | _BV(CPHA);
	SPSR = _BV(SPI2X);
}

void spi_transmit(char value){
    /*transmitting the value to the slave/shift register*/
	SPDR = value;
}
