#ifndef __LCD_H
#define __LCD_H

#include "stm32f1xx_hal.h"

// constant
#define LCD_COMMAND     0x00
#define LCD_DATA        0x01

// commands
#define LCD_CLEAR_DISPLAY                0x01
#define LCD_RETURN_HOME                  0x02

#define LCD_ENTRY_MODE_SET               0x04
#define LCD_DISPLAY_CONTROL              0x08
#define LCD_CURSOR_SHIFT                 0x10
#define LCD_FUNCTION_SET 				 0x20
#define LCD_SET_CGRAMADDR 				 0x40
#define LCD_SET_DDRAMADDR	 			 0x80

// flags for display entry mode
#define LCD_ENTRY_RIGHT 				 0x00
#define LCD_ENTRY_LEFT 					 0x02
#define LCD_ENTRY_SHIFT_INCREMENT        0x01
#define LCD_ENTRY_SHIFT_DECREMENT        0x00

// flags for display on/off control
#define LCD_DISPLAY_ON 					 0x04
#define LCD_DISPLAY_OFF 				 0x00
#define LCD_CURSOR_ON 					 0x02
#define LCD_CURSOR_OFF 					 0x00
#define LCD_BLINK_ON 					 0x01
#define LCD_BLINK_OFF 					 0x00

// flags for display/cursor shift
#define LCD_DISPLAY_MOVE 				 0x08
#define LCD_CURSOR_MOVE 				 0x00
#define LCD_MOVE_RIGHT 					 0x04
#define LCD_MOVE_LEFT 					 0x00

// flags for function set
#define LCD_8BITMODE 					 0x10
#define LCD_4BITMODE 					 0x00
#define LCD_2LINE 						 0x08
#define LCD_1LINE 						 0x00
#define LCD_5x10DOTS 					 0x04
#define LCD_5x8DOTS 					 0x00

typedef struct
{
	uint8_t MODE;
	uint8_t COLUMS; 
	uint8_t ROWS; 
	GPIO_TypeDef* RS_PORT;
	uint16_t RS_PIN;
	GPIO_TypeDef* EN_PORT;
	uint16_t EN_PIN;
	GPIO_TypeDef* D0_PORT;
	uint16_t D0_PIN;
	GPIO_TypeDef* D1_PORT;
	uint16_t D1_PIN;
	GPIO_TypeDef* D2_PORT;
	uint16_t D2_PIN;
	GPIO_TypeDef* D3_PORT;
	uint16_t D3_PIN;
	GPIO_TypeDef* D4_PORT;
	uint16_t D4_PIN;
	GPIO_TypeDef* D5_PORT;
	uint16_t D5_PIN;
	GPIO_TypeDef* D6_PORT;
	uint16_t D6_PIN;
	GPIO_TypeDef* D7_PORT;
	uint16_t D7_PIN;
	uint8_t ENTRYMODE;
	uint8_t DISPLAYCTRL;
	uint8_t CURSORSHIFT;
	uint8_t FUNCTIONSET;
}LCD_Name;

void lcd_init(LCD_Name* LCD, uint8_t colum, uint8_t row,
									GPIO_TypeDef* RS_PORT, uint16_t RS_PIN, GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
									GPIO_TypeDef* D0_PORT, uint16_t D0_PIN, GPIO_TypeDef* D1_PORT, uint16_t D1_PIN,
									GPIO_TypeDef* D2_PORT, uint16_t D2_PIN, GPIO_TypeDef* D3_PORT, uint16_t D3_PIN,
									GPIO_TypeDef* D4_PORT, uint16_t D4_PIN, GPIO_TypeDef* D5_PORT, uint16_t D5_PIN,
									GPIO_TypeDef* D6_PORT, uint16_t D6_PIN, GPIO_TypeDef* D7_PORT, uint16_t D7_PIN);

void lcd_set_cursor(LCD_Name* LCD, uint8_t colum, uint8_t row);
void lcd_write_char(LCD_Name* LCD, char character);
void lcd_write_string(LCD_Name* LCD, char *String);
void lcd_clear(LCD_Name* LCD);
