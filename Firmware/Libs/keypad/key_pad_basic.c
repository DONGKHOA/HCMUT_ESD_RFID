#include "key_pad_basic.h"

#define ROW_1_Pin GPIO_PIN_0
#define ROW_2_Pin GPIO_PIN_1
#define ROW_3_Pin GPIO_PIN_2
#define ROW_4_Pin GPIO_PIN_3
#define COL_1_Pin GPIO_PIN_4
#define COL_2_Pin GPIO_PIN_5
#define COL_3_Pin GPIO_PIN_6

GPIO_TypeDef* ROW_1_Port = GPIOA;
GPIO_TypeDef* ROW_2_Port = GPIOA;
GPIO_TypeDef* ROW_3_Port = GPIOA;
GPIO_TypeDef* ROW_4_Port = GPIOA;
GPIO_TypeDef* COL_1_Port = GPIOA;
GPIO_TypeDef* COL_2_Port = GPIOA;
GPIO_TypeDef* COL_3_Port = GPIOA;


void keypad_init(void)
{
  // Configure GPIO pins for keypad matrix
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ROW_1_Pin | ROW_2_Pin | ROW_3_Pin | ROW_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ROW_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_3_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_4_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = COL_1_Pin | COL_2_Pin | COL_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(COL_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_3_Port, &GPIO_InitStruct);
}

uint8_t keypad_scan()
{
  for(int i = 0; i < 3; i++)
  {
    // Set current column as output and low
    switch(i)
    {
      case 0:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
        break;
        
      case 1:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
        break;
      
      case 2:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_RESET);
        break;
    }

  // Read current rows
  if(HAL_GPIO_ReadPin(ROW_1_Port, ROW_1_Pin) == GPIO_PIN_RESET)
      return i;
  if(HAL_GPIO_ReadPin(ROW_2_Port, ROW_2_Pin) == GPIO_PIN_RESET)
      return i + 3;
  if(HAL_GPIO_ReadPin(ROW_3_Port, ROW_3_Pin) == GPIO_PIN_RESET)
      return i + 6;
  if(HAL_GPIO_ReadPin(ROW_4_Port, ROW_4_Pin) == GPIO_PIN_RESET)
      return i + 9;
  }
  return 13;
}

char keypad_handle ()
{
	char keys[12] = {'1','2','3','4','5','6','7','8','9','*','0','#'};
	uint8_t key_current;
	uint8_t key_last = 13;
	uint8_t key_debouncing = 13;
	uint8_t is_debouncing = 0;
	uint32_t debounce_timer;

	uint8_t key = keypad_scan();
	// bouncing
	if(key != key_debouncing)
	{
		is_debouncing = 1;
		debounce_timer = HAL_GetTick();
		key_debouncing = key;
	}

	// debouncing successfull
	if(is_debouncing == 1 && (HAL_GetTick() - debounce_timer >= 15))
	{
		key_current = key_debouncing;
		is_debouncing = 0;
	}

	if(key_current != key_last)
	{
		if(key_current != 13)
		{
//			keypad_pressing_callback(key_current);
			return keys[key];
		}
		key_last = key_current;
	}
}

//char keypad_pressing_callback (uint8_t key)
//{
//
//}

