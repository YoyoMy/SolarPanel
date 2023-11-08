/*
* SolarPanelControl.c
*
* Created: 02/05/2023 12.43.15
* Author : Khaled & Yoana
*/

#include <math.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keyboard_driver/matrix_keyboard_driver.h"
#include "timer/timer_driver.h"
#include "key_drivers/AbstractKeys.h"
#include "display_driver/display_driver.h"
#include "led_driver/led_driver.h"
#include "spi_driver/spi_driver.h"
#include "tmp36_driver/tmp36_driver.h"

#define F_CPU 16000000L


// Variable storing the converted temperature from the driver
float temperature;
// Low threshold - default value 20.5
float LOW_t = 20.5;
// High threshold - default value 30.5
float HIGH_t = 30.5;
// Helper variable for read_input method, also displayed when editing the thresholds
float value = 0.0;
// Flag for displaying the number entered while editing HIGH_t or LOW_t
int editing_values = 0;
// Variable for storing the switch pressed
static uint8_t key_pressed = 0;

void operate_pump()
{
	(temperature > HIGH_t) ? set_led(7, 1) : set_led(7, 0);
}

void operate_valve()
{
	(temperature < LOW_t) ? set_led(6, 1) : set_led(6, 0);
}

void display_temperature()
{
	if (editing_values == 1)
	{
		printfloat_4f(value, 1);
	}
	else if (key_pressed == 1)
	{
		printfloat_4f(LOW_t, 1);
	}
	else if (key_pressed == 2)
	{
		printfloat_4f(HIGH_t, 1);
	}
	else {
		printfloat_4f(temperature, 1);
	}
}

void convert_temperature()
{
	uint16_t measured = get_measurement();
	// Convert the ADC value to temperature in degrees Celsius
	temperature = ((measured * (5000.0 / 1024.0)) - 500) / 10.0;
	operate_pump();
	operate_valve();
}

float read_input() {
	// The whole part of the decimal number
	float whole_num = 0.0;
	// The value after the decimal point
	float decimal_num = 0.0;
	// For determining where to place the decimal point
    int num_dicimals = 0;
	// Flag variable to track if a key has been pressed in the current digit
	int key_pressed = 0;  
	int key = -1;
	// Flag that decimal point has not been pressed ("D" on matrix keypad)
	int decimal_point = 0;
    while (key != 14 && key != 15) {
        // Scan the matrix to check if any key is pressed
        key = getxkey();
		
		// Reset the flag if the no press has been scanned
		if (key_pressed == 1 && key == -1)
		{
			key_pressed = 0;
		}
		//Set the flag for the decimal point
		if (key == 13)
		{
			decimal_point = 1;
		}
		// Logic for the whole part of the number
        if (key >= 0 && key <= 9 && key_pressed == 0 && decimal_point == 0) {
            // A key is pressed for the first time in this digit, so we update the input value and set the flag
            key_pressed = 1;
            whole_num = whole_num * 10 + key;
			value = whole_num;
        }
		// Logic for the decimal part of the number
		else if (key >= 0 && key <= 9 && key_pressed == 0 && decimal_point == 1) {
			// A key is pressed for the first time in this digit, so we update the input value and set the flag
			key_pressed = 1;
			num_dicimals++;
			decimal_num = decimal_num * 10 + key;
			value = (float)whole_num + ((float)decimal_num / pow(10, num_dicimals));
		}
    }
	if (key == 14) //"*" on keypad
	{
		//dismiss value
		value = -1;
	}
	// Reset the flag
	decimal_point = 0;
    return value;
}

void clear_leds()
{
    set_led(3, 0);
    set_led(2, 0);
    set_led(1, 0);
}

void update_leds()
{
	clear_leds();
	set_led(key_pressed, 1);
}

int main(void)
{
	sei();
	init_spi();
	init_display();
	timer1_start();
	timer0_start(display_temperature);
	tmp36_init(convert_temperature);
	tmp36_measure();
	init_matrix_keyboard();
	init_leds();
	init_keys();
	set_led(3, 1);
	
	while (1)
	{
		if (scan_key() != 0)
		{
			key_pressed = scan_key();
		}
		if (key_pressed == 3)
		{
			update_leds();
		}
		if (key_pressed == 1 || key_pressed == 2)
		{
			update_leds();
			int get_key = getxkey();
			if (get_key >=1 && get_key <= 9)
			{
				editing_values = 1;
				value = read_input();
				if (value != -1)
				{
					(key_pressed == 1) ? (LOW_t = value) : (HIGH_t = value);
				}
				editing_values = 0;
			}
		}
	}
}

