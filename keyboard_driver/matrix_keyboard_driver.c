/* 
 * File:   matrix_keyboard_driver.c
 * Author: yoana
 *
 * Created on April 28, 2023, 9:08 AM
 */
#include "stdint.h"
#include "avr/io.h"
#define F_CPU 16000000L

typedef struct key_descr_t
{
	 volatile uint8_t* ddr;
	 volatile uint8_t* pin;
	 volatile uint8_t* port;
	 uint8_t bit;
} key_descr_t;

static key_descr_t cols[] = {
    {&DDRE, &PINE, &PORTE, PE5},
    {&DDRH, &PINH, &PORTH, PH3},
    {&DDRH, &PINH, &PORTH, PH5},
    {&DDRL, &PINL, &PORTL, PL1}

};

static key_descr_t rows[] = {
    {&DDRB, &PINB, &PORTB, PB7},
    {&DDRB, &PINB, &PORTB, PB5},
    {&DDRB, &PINB, &PORTB, PB4},
    {&DDRE, &PINE, &PORTE, PE3}
};

int8_t _keypad[4][4] = {{1,2,3,10},
                        {4,5,6,11},
                        {7,8,9,12},
                        {14,0,15,13}
};

void init_matrix_keyboard()
{
    // Initialize input-pin for keys
	for(uint8_t i=0; i<4; i++)
	{
		// Initialize Data Direction Register for input
		*(cols[i].ddr) &= ~_BV(cols[i].bit);
		*(cols[i].port) |= _BV(cols[i].bit);
        
        // Initialize Data Direction Register for output
        *(rows[i].ddr) |= _BV(rows[i].bit);
        *(rows[i].port) |= _BV(rows[i].bit);
	}
}

int8_t row, col;

int8_t getxkey()
{
	row = -1, col = -1;
    for(int i = 0; i<4; i++)
    {
        
        //set cols to low
        *(rows[i].port) &= ~_BV(rows[i].bit);
            for(int j = 0; j<4; j++)
            {
                
                if((*(cols[j].pin) & _BV(cols[j].bit))==0)
                {
                    row = i;
                    col = j;
                }
            }
        *(rows[i].port) |= _BV(rows[i].bit);
    }
	if (row == -1 && col == -1)
		return -1;
		
    return _keypad[row][col];
}