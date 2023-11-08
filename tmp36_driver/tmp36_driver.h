
/*
 * tmp36_driver.h
 *
 * Created: 22/03/2023 09.35.57
 *  Authors : haled & yoana
 */ 

void tmp36_init(void (*cb)());
void tmp36_measure();
uint16_t get_measurement();