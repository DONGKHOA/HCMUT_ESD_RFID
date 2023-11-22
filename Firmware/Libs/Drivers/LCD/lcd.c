#include "lcd.h"

static void delay_us(uint32_t us)
{
	htim2.Instance->CNT = 0;
	HAL_TIM_Base_Start(&htim2);
	while(htim2.Instance->CNT < us);
	HAL_TIM_Base_Stop(&htim2);
}

static void lcd_write_data(LCD_Name* LCD, uint8_t data, uint8_t mode)
{
	if (mode == LCD_COMMAND)
	{
		HAL_GPIO_WritePin(LCD->RS_PORT, LCD->RS_PIN, GPIO_PIN_RESET); 	//RS = 0, write cmd
	}
	else if (mode == LCD_DATA)
	{
		HAL_GPIO_WritePin(LCD->RS_PORT, LCD->RS_PIN, GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(LCD->D0_PORT, LCD->D0_PIN, data & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D1_PORT, LCD->D1_PIN, data >> 1&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D2_PORT, LCD->D2_PIN, data >> 2&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D3_PORT, LCD->D3_PIN, data >> 3&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D4_PORT, LCD->D4_PIN, data >> 4&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D5_PORT, LCD->D5_PIN, data >> 5&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D6_PORT, LCD->D6_PIN, data >> 6&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D7_PORT, LCD->D7_PIN, data >> 7&0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	delay_us(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_SET);
	delay_us(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	delay_us(1);
}

void lcd_init(LCD_Name* LCD, uint8_t colum, uint8_t row,
									GPIO_TypeDef* RS_PORT, uint16_t RS_PIN, GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
									GPIO_TypeDef* D0_PORT, uint16_t D0_PIN, GPIO_TypeDef* D1_PORT, uint16_t D1_PIN,
									GPIO_TypeDef* D2_PORT, uint16_t D2_PIN, GPIO_TypeDef* D3_PORT, uint16_t D3_PIN,
									GPIO_TypeDef* D4_PORT, uint16_t D4_PIN, GPIO_TypeDef* D5_PORT, uint16_t D5_PIN,
									GPIO_TypeDef* D6_PORT, uint16_t D6_PIN, GPIO_TypeDef* D7_PORT, uint16_t D7_PIN)
{
	LCD->MODE = LCD_8BITMODE;
	LCD->COLUMS = colum;
	LCD->ROWS = row;
	LCD->RS_PORT = RS_PORT;
	LCD->RS_PIN = RS_PIN;
	LCD->EN_PORT = EN_PORT;
	LCD->EN_PIN = EN_PIN;
	LCD->D0_PORT = D0_PORT;
	LCD->D0_PIN = D0_PIN;
	LCD->D1_PORT = D1_PORT;
	LCD->D1_PIN = D1_PIN;
	LCD->D2_PORT = D2_PORT;
	LCD->D2_PIN = D2_PIN;
	LCD->D3_PORT = D3_PORT;
	LCD->D3_PIN = D3_PIN;
	LCD->D4_PORT = D4_PORT;
	LCD->D4_PIN = D4_PIN;
	LCD->D5_PORT = D5_PORT;
	LCD->D5_PIN = D5_PIN;
	LCD->D6_PORT = D6_PORT;
	LCD->D6_PIN = D6_PIN;
	LCD->D7_PORT = D7_PORT;
	LCD->D7_PIN = D7_PIN;
	delay_us(50);
	LCD->FUNCTIONSET = LCD_FUNCTION_SET|LCD_8BITMODE|LCD_2LINE|LCD_5x8DOTS;
	LCD->ENTRYMODE = LCD_ENTRY_MODE_SET|LCD_ENTRY_LEFT|LCD_ENTRY_SHIFT_DECREMENT;
	LCD->DISPLAYCTRL = LCD_DISPLAY_CONTROL|LCD_DISPLAY_ON|LCD_CURSOR_OFF|LCD_BLINK_OFF;
	LCD->CURSORSHIFT = LCD_CURSOR_SHIFT|LCD_CURSOR_MOVE|LCD_MOVE_RIGHT;

	lcd_write_data(LCD, LCD->ENTRYMODE, LCD_COMMAND);
	lcd_write_data(LCD, LCD->DISPLAYCTRL, LCD_COMMAND);
	lcd_write_data(LCD, LCD->CURSORSHIFT, LCD_COMMAND);
	lcd_write_data(LCD, LCD->FUNCTIONSET, LCD_COMMAND);
	
	lcd_write_data(LCD, LCD_CLEAR_DISPLAY, LCD_COMMAND);
	lcd_write_data(LCD, LCD_RETURN_HOME, LCD_COMMAND);
}

void lcd_set_cursor(LCD_Name* LCD, uint8_t colum, uint8_t row)
{
	uint8_t DRAM_ADDRESS = 0x00;
	uint8_t DRAM_OFFSET[4] = {0x00, 0x40, 0x14, 0x54};
	if(colum >= LCD->COLUMS)
	{
		colum = LCD->COLUMS - 1;
	}
	if(row >= LCD->ROWS)
	{
		row = LCD->ROWS -1;
	}
	DRAM_ADDRESS = DRAM_OFFSET[row] + colum;
	lcd_write_data(LCD, LCD_SET_DDRAMADDR | DRAM_ADDRESS, LCD_COMMAND);
}

void lcd_write_char(LCD_Name* LCD, char character)
{
	lcd_write_data(LCD, character, LCD_DATA);
}

void lcd_write_string(LCD_Name* LCD, char *String)
{
	while(*String)lcd_write_char(LCD, *String++);
}

void lcd_clear(LCD_Name* LCD)
{
	lcd_write_data(LCD, LCD_CLEAR_DISPLAY, LCD_COMMAND);
	delay_us(5);
}
