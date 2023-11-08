/*
 * thermometer.h
 *
 * Created: 3/23/2023 3:59:59 AM
 *  Authors : haled & yoana
 */ 

#include "thermometer.h"
#include <avr/io.h>
#include "avr/interrupt.h"
#include "tmp36_driver.h"
#include "timer_driver.h"
#include "display_driver.h"
#include "spi_driver.h"
#include <stdio.h>

#define F_CPU 16000000L

/*void display_temperature()
{
    printfloat_4f(temperature, 1);
}

void convert_temperature()
{
	uint16_t measured = get_measurement();
	// Convert the ADC value to temperature in degrees Celsius
	temperature = ((measured * (5000.0 / 1024.0)) - 500) / 10.0;  
    //display_temperature();
}*/

void initialize_thermometer()
{
	/*init_spi();
	init_display();
	timer1_start();
	timer0_start(display_temperature);
	tmp36_init(convert_temperature);
	tmp36_measure();*/
}
