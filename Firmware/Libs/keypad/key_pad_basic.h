#ifndef	_KEYPAD_H
#define	_KEYPAD_H

#include <stdint.h>

#include "stm32f1xx_hal.h"


void keypad_init(void);
char keypad_handle ();
uint8_t keypad_scan();

#endif
