/*
 * AbstractKeys.c
 *
 * Created: 21/04/2022
 *  Author: ERL
 */ 
#include <avr/io.h>

typedef struct key_descr_t
{
	 volatile uint8_t* ddr;
	 volatile uint8_t* pin;
	 volatile uint8_t* port;
	 uint8_t bit;
} key_descr_t;

/*
Key register addresses in I/O map
Copy I/O register addresses (DDRx, PINx, PORTx, port bit) into key descriptor array
*/
static key_descr_t key[] = {
	{&DDRC, &PINC, &PORTC, PC0},	// Key1
	{&DDRC, &PINC, &PORTC, PC1},	// Key2
	{&DDRC, &PINC, &PORTC, PC2},	// Key3
	{&DDRC, &PINC, &PORTC, PC3},	// Key4
	{&DDRC, &PINC, &PORTC, PC4},	// Key5
	{&DDRC, &PINC, &PORTC, PC5},	// Key6
	{&DDRD, &PIND, &PORTD, PD2},	// Key7
	{&DDRD, &PIND, &PORTD, PD3}		// Key8
	};

void init_keys()
{
	// Initialize input-pin for keys
	for(uint8_t i=0; i<8; i++)
	{
		// Initialize Data Direction Register for each key-pin
		*(key[i].ddr) &= ~_BV(key[i].bit);
		
		// Enable Pull-ups on each key input
		*(key[i].port) |= _BV(key[i].bit);
	}
}


uint8_t get_key(uint8_t key_no)
{
	if((key_no >= 1) && (key_no <= 8))
	{
		key_no--;	// Convert key number to index

		// Evaluate if key is activated (Active low)
		return ((*(key[key_no].pin) & _BV(key[key_no].bit))==0);
	}
	return 0;
}

uint8_t scan_key()
{
	for(uint8_t i=1; i<=8; i++)
	{
		if(get_key(i))
		{
			return i;
		}
	}
	return 0;
}