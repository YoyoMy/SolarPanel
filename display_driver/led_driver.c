
/*
 * led_driver.c
 *
 * Created: 2/24/2023 12:53:35 AM
 *  Author: haled & yoana
 */
#include <avr/io.h>
#define F_CPU 16000000L

typedef struct led_descr_t
{
	volatile uint8_t *ddr;
	volatile uint8_t *pin;
	volatile uint8_t *port;
	uint8_t bit;
} led_descr_t;

static led_descr_t led[] = {
	{&DDRA, &PINA, &PORTA, PA0}, // Led1
	{&DDRA, &PINA, &PORTA, PA1}, // Led2
	{&DDRA, &PINA, &PORTA, PA2}, // Led3
	{&DDRA, &PINA, &PORTA, PA3}, // Led4
	{&DDRA, &PINA, &PORTA, PA4}, // Led5
	{&DDRA, &PINA, &PORTA, PA5}, // Led6
	{&DDRA, &PINA, &PORTA, PA6}, // Led7
	{&DDRA, &PINA, &PORTA, PA7}	 // Led8
};

void init_leds()
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// Configure PORTA as output for the LEDs
		*(led[i].ddr) |= _BV(led[i].bit);
		// Turn off all LEDs
		*(led[i].port) |= _BV(led[i].bit);
	}
}

void set_led(uint8_t led_no, uint8_t state)
{
	if ((led_no >= 1) && (led_no <= 8))
	{
		led_no--;
		if (state == 0)
			*(led[led_no].port) |= _BV(led[led_no].bit);
		else
			*(led[led_no].port) &= ~_BV(led[led_no].bit);
	}
}

// Levels 0...8
void set_bar(uint8_t level)
{
	if (level <= 8)
	{
		// Clear all LEDs
		for (uint8_t i = 0; i < 8; i++)
		{
			*(led[i].port) |= _BV(led[i].bit);
		}

		// Set specified number of leds
		for (int i = 0; i <= level; i++)
			set_led(i, 1);
	}
}