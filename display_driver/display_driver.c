/*
 * display_driver.c
 *
 * Created: 14/04/2023 09.05.11
 *  Authors : haled & yoana
 */

#include "avr/io.h"
#include "spi_driver.h"
#include <util/delay.h>

#define F_CPU 16000000L

int index = 0; // initialize index to the first value
static char numbers[] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09};
static char empty = 0xFF;
	
void init_display(){
	////initialize
	/// setting pins 0, 1, 2, 3, on PORTF to inputs
    /// those pins define which display to be turned on (if it is set to 0)
	DDRF |= _BV(PF0) | _BV(PF1) | _BV(PF2) | _BV(PF3);
	PORTF |= _BV(PF0) | _BV(PF1) | _BV(PF2) | _BV(PF3);
}

void printint_4u(uint16_t value)
{
    //split the value into digits
	char temps[4];
	for (int i = 3; i >= 0; i--)
	{
		temps[i] = value % 10;
		value /= 10;
	}
	//move the value from the shift register to the display
	PORTB |= _BV(PB0);
	//clear the displays
	PORTF |= _BV(PF0) | _BV(PF1) | _BV(PF2) | _BV(PF3);
    //set which display should show the value
	PORTF &= ~(_BV(index));
	//stop transmitting form the shift register
	PORTB &= ~(_BV(PB0));
    //move to the next index
	index = (index + 1) % 4;
    //get the next value to be displayed
	int number_index = temps[index];
    //check if it is the first 2 digits are zero, then do not lit up the display
    if(index == 0 || index == 1 && number_index == 0)
    {
        //start transmission to the shift register using SPI
        spi_transmit(empty);
    }
    //start transmission to the shift register using SPI
	else spi_transmit(numbers[number_index]);
}

void printfloat_4f(float value, uint8_t no_of_decimals)
{
    //25,4 -> 254
    int temp = value * 10;
	//split the value into digits
	char temps[4];
	for (int i = 3; i >= 0; i--)
	{
		temps[i] = temp % 10;
		temp /= 10;
	}
    //move the value from the shift register to the display
	PORTB |= _BV(PB0);
	//clear the displays
	PORTF |= _BV(PF0) | _BV(PF1) | _BV(PF2) | _BV(PF3);
    //set which display should show the value
	PORTF &= ~(_BV(index));
	//stop transmitting form the shift register
	PORTB &= ~(_BV(PB0));
    //get on which position we should add the deciaml point
    int dot_index = 3 - no_of_decimals;
    //move to the next index
	index = (index + 1) % 4;
    //get the next value to be displayed
	int number_index = temps[index];
    //check if it is the first 1 digits is zero, then do not lit up the display
    if(index == 0 && number_index == 0 )
    {
        //start transmission to the shift register using SPI
        spi_transmit(empty);
    }
    //check if it we should add decimal point after the digit
    else if(index == dot_index)
    {
        //start transmission to the shift register using SPI
        spi_transmit(numbers[number_index] - 1);
    }
    //start transmission to the shift register using SPI
	else spi_transmit(numbers[number_index]);
}