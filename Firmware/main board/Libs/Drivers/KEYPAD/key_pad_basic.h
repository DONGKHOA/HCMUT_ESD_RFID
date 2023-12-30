#ifndef	_KEYPAD_H
#define	_KEYPAD_H

#include <stdint.h>

#include "stm32f1xx_hal.h"

typedef enum
{
	PRESSING_STATE,
	NONE_PRESSING_STATE
}keyPad_State_t;

void keypad_init(void);
uint8_t keypad_handle ();
uint8_t keypad_scan();

#endif
