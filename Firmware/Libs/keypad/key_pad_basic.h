#ifndef	_KEYPAD_H
#define	_KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

#define ROW_1_Pin GPIO_PIN_6
#define ROW_2_Pin GPIO_PIN_5
#define ROW_3_Pin GPIO_PIN_4
#define ROW_4_Pin GPIO_PIN_3
#define COL_1_Pin GPIO_PIN_2
#define COL_2_Pin GPIO_PIN_1
#define COL_3_Pin GPIO_PIN_0

GPIO_TypeDef* ROW_1_Port = GPIOA;
GPIO_TypeDef* ROW_2_Port = GPIOA;
GPIO_TypeDef* ROW_3_Port = GPIOA;
GPIO_TypeDef* ROW_4_Port = GPIOA;
GPIO_TypeDef* COL_1_Port = GPIOA;
GPIO_TypeDef* COL_2_Port = GPIOA;
GPIO_TypeDef* COL_3_Port = GPIOA;

char keys[12] = {'1','2','3','4','5','6','7','8', '9','*','0','#'};

int button_debouncing (GPIO_TypeDef *GPIO_Button, uint16_t GPIO_Pin_Button);
int keypad_scan(void);
char keypad_get_char ( void );

#endif