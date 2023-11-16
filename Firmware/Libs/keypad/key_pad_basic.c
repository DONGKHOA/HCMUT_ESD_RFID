#include "stm32f4xx_hal.h"
#include "key_pad_basic.h"

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
  
  GPIO_InitStruct.Pin = COL_1_Pin | COL_2_Pin | COL_3_Pin | COL_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(COL_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_3_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_4_Port, &GPIO_InitStruct);
}

int button_debouncing (GPIO_TypeDef *GPIO_Button, uint16_t GPIO_Pin_Button)
{
  uint8_t current_state = 1;
  uint8_t last_state = 1;
  uint8_t deboucing_state = 1;
  uint8_t is_deboucing = 0;
  uint32_t deboucing_timer = 0;

	// detecting
	uint8_t temp_state = HAL_GPIO_ReadPin(GPIO_Button, GPIO_Pin_Button);
	if (temp_state != deboucing_state)
	{
		deboucing_state = temp_state;
		deboucing_timer = HAL_GetTick();
		is_deboucing = 1;
	}

	// deboucing
	if (is_deboucing == 1 && (HAL_GetTick() - deboucing_timer) > 15)
	{
		current_state = deboucing_state;
		is_deboucing = 0;
	}

	if (current_state != last_state)
	{
		return 1;      // pressing
	}
  if (current_state == last_state)
  {
    return 0;     // none pressing
  }
  
}

int keypad_scan(void)
{                    
  for(int i = 0; i < 4; i++)
  {
    // Set current column as output and low
    switch(i)
    {
      case 0:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
        break;
        
      case 1:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
        break;
      
    case 2:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
        break;
      
    case 3:
        HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_RESET);
        break;
  }

  // Read current rows
  if(HAL_GPIO_ReadPin(ROW_1_Port, ROW_1_Pin) == GPIO_PIN_RESET)
  {
    if (button_debouncing(ROW_1_Port, ROW_1_Pin) == 1)
    {
      return i;
    }
    
  }
  if(HAL_GPIO_ReadPin(ROW_2_Port, ROW_2_Pin) == GPIO_PIN_RESET)
  {
    if (button_debouncing(ROW_2_Port, ROW_2_Pin) == 1)
    {
      return i + 3;
    }
  }
  if(HAL_GPIO_ReadPin(ROW_3_Port, ROW_3_Pin) == GPIO_PIN_RESET)
  {
    if (button_debouncing(ROW_3_Port, ROW_3_Pin) == 1)
    {
      return i + 6;
    }
  }
  if(HAL_GPIO_ReadPin(ROW_4_Port, ROW_4_Pin) == GPIO_PIN_RESET)
    if (button_debouncing(ROW_4_Port, ROW_4_Pin) == 1)
    {
      return i + 9;
    }
  }
  return 0; // No key pressed
}

char keypad_get_char ( void )
{
  int key = keypad_scan ();
  return keys[key];
}


