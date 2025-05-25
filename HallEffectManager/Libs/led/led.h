#ifndef LED_H
#define LED_H

#include "main.h"

//First bit defines if program should stop, 1 for stop, 0 for continue, each bit corresponds to GPIO pi DEBUG_LED, on or off for 100ms
//700ms per led call

#define STATUS_OK 0x50 //01010000


void status_led(uint8_t code);




#endif