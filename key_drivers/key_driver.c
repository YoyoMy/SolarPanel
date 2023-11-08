
/*
 * key_driver.c
 *
 * Created: 2/24/2023 12:54:18 AM
 *  Author: haled & yoana
 */
#include <avr/io.h>
#define F_CPU 16000000L

void init_keys()
{
    DDRC = 0;
    PORTC = 0xFF;
}
uint8_t get_key(uint8_t key_no)
{
    if (key_no < 1 || key_no > 8)
        return 0;
    key_no--;
    return (PORTC & _BV(key_no)) == 0 ? 1 : 0;
}