
/*
 * led_driver.h
 *
 * Created: 2/24/2023 12:53:50 AM
 *  Author: haled & yoana
 */
#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

void init_leds();
void set_led(uint8_t led_no, uint8_t state);
void set_bar(uint8_t level); // Levels 0...8. 0=off

#endif
