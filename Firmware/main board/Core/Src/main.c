/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "menu_mode.h"
#include "lcd.h"
#include "key_pad_basic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  INIT_STATE = 0,
  OPEN_DOOR_STATE,
  PASSWORD_IDENTIFICATION,
  USER_MODE_KEYPAD,
  USER_MODE_CARD,
  ADMIN_MODE,
  ADMINISTRATOR,
  CHANGE_PASSWORD_ADMIN,
  UNBLOCK_USER,
  CHANGE_PASSWORD_USER,
  ADD_NEW_CARD,
  BLOCK_USER_STATE,
  BLOCK_ADMIN_STATE
} state_handle_t;

state_handle_t state_task = INIT_STATE;
state_door_t state_door;
uint8_t data_keypad[15];
uint8_t data_temp;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t ENTER_Password(uint8_t *data_keypad)
{
  static uint8_t dest = 0;
  uint8_t data = 0;
  data = keypad_handle();
  if (data != NONE_PRESSING_STATE)
  {
    if ((data == '#') || (data == '*'))
    {
      dest = 0;
      return data;
    }
    *(data_keypad + dest) = data;
    dest++;
  }
  return 0;
}

void task_handle(void)
{
  switch (state_task)
  {
  case INIT_STATE:
    state_door = Init_Door();
    if (state_door == BLOCK_USER)
    {
      state_task = BLOCK_USER_STATE;
    }
    else
    {
      state_task = PASSWORD_IDENTIFICATION;
    }

    break;

  case OPEN_DOOR_STATE:
    OPEN_DOOR();
    // turn off backlight
    state_task = PASSWORD_IDENTIFICATION;
    break;

  case PASSWORD_IDENTIFICATION:
    data_temp = ENTER_Password(data_keypad);
    if (data_temp != 0)
    {
      if (data_temp == '#')
      {
        state_door = Password_identification(data_keypad);
        if (state_door == USER_ACCESS)
        {
          state_task = USER_MODE_KEYPAD;
        }
        else
        {
          state_task = ADMIN_MODE;
        }
      }
      else if (data_temp == '*')
      {
        state_task = INIT_STATE;
      }
    }

  case USER_MODE_CARD:
    // check uid card
    break;

  case USER_MODE_KEYPAD:
    state_door = USER_Mode_Keypad(data_keypad);
    memset(data_keypad, 0, sizeof(data_keypad));
    if (state_door == OPEN_DOOR)
    {
      state_task = OPEN_DOOR_STATE;
    }
    else if (state_door == CLOSE_DOOR)
    {
      state_task = PASSWORD_IDENTIFICATION;
    }
    else
    {
      state_task = BLOCK_USER_STATE;
    }
    break;

  case ADMIN_MODE:
    state_door = ADMIN_Mode(data_keypad);
    memset(data_keypad, 0, sizeof(data_keypad));
    if (state_door == ADMIN_ACCESS_SUCCESSFULLY)
    {
      data_temp = ENTER_Password(data_keypad);
      if (data_temp == '#')
      {
        if (*data_keypad == '1')
        {
          state_task = ADMINISTRATOR;
        }
        else if (*data_keypad == '2')
        {
          state_task = CHANGE_PASSWORD_USER;
        }
        else
        {
          state_task = ADD_NEW_CARD;
        }
      }
      else if (data_temp == '*')
      {
        state_task = PASSWORD_IDENTIFICATION;
      }
    }
    else if (state_door == ADMIN_ACCESS_FAILED)
    {
      state_task = PASSWORD_IDENTIFICATION;
    }
    else
    {
      state_task = BLOCK_ADMIN_STATE;
    }
    break;

  case ADMINISTRATOR:
    uint8_t data = 0;
    data = _Administrator();
    if(data == 1) state_task = CHANGE_PASSWORD_ADMIN;
    if(data == 1) state_task = UNBLOCK_USER;
    break;

  case CHANGE_PASSWORD_ADMIN:
    data_temp = ENTER_Password(data_keypad);
    if (data_temp != 0)
    {
      state_door = __Change_password_admin(data_keypad, data_temp);
      if (state_door == STATE_END)
      {
        HAL_Delay(500);
        state_task = ADMIN_MODE;
      }
      else if (state_door == STATE_FORWARD)
      {
        state_task = ADMINISTRATOR;
      }
    }
    break;

  case UNBLOCK_USER:
    state_door = __Unblock_User();
    if (state_door == STATE_END)
    {
      HAL_Delay(500);
      state_task = ADMIN_MODE;
    }
    else if (state_door == STATE_FORWARD)
    {
      state_task = ADMINISTRATOR;
    }
    break;

  case CHANGE_PASSWORD_USER:
    data_temp = ENTER_Password(data_keypad);
    if (data_temp != 0)
    {
      state_door = _Change_password_user(data_keypad, data_temp);
      if (state_door == STATE_END)
      {
        HAL_Delay(500);
        state_task = ADMIN_MODE;
      }
    }
    break;

  case ADD_NEW_CARD:

    break;

  case BLOCK_USER_STATE:
    DISPLAY_Block_User(&lcd_0);
    HAL_Delay(500);
    state_task = PASSWORD_IDENTIFICATION;
    break;

  case BLOCK_ADMIN_STATE:
    DISPLAY_Block_Admin(&lcd_0);
    break;
  default:
    #error "No State access"
    break;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DRIVER_BUZZER_Pin|DRIVER_LED_GRN_Pin|DRIVER_LED_RED_Pin|DRIVER_LOCK_Pin
                          |LCD_D7_Pin|LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |LCD_D3_Pin|LCD_D2_Pin|LCD_D1_Pin|LCD_D0_Pin
                          |LCD_EN_Pin|LCD_RW_Pin|LCD_RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, KEYPAD_C2_Pin|KEYPAD_C1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LCD_LED_Pin|KEYPAD_C3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRIVER_BUZZER_Pin DRIVER_LED_GRN_Pin DRIVER_LED_RED_Pin DRIVER_LOCK_Pin
                           LCD_D7_Pin LCD_D6_Pin LCD_D5_Pin LCD_D4_Pin
                           LCD_D3_Pin LCD_D2_Pin LCD_D1_Pin LCD_D0_Pin
                           LCD_EN_Pin LCD_RW_Pin LCD_RS_Pin */
  GPIO_InitStruct.Pin = DRIVER_BUZZER_Pin|DRIVER_LED_GRN_Pin|DRIVER_LED_RED_Pin|DRIVER_LOCK_Pin
                          |LCD_D7_Pin|LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |LCD_D3_Pin|LCD_D2_Pin|LCD_D1_Pin|LCD_D0_Pin
                          |LCD_EN_Pin|LCD_RW_Pin|LCD_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KEYPAD_C2_Pin KEYPAD_C1_Pin */
  GPIO_InitStruct.Pin = KEYPAD_C2_Pin|KEYPAD_C1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : KEYPAD_R4_Pin KEYPAD_R3_Pin KEYPAD_R2_Pin KEYPAD_R1_Pin */
  GPIO_InitStruct.Pin = KEYPAD_R4_Pin|KEYPAD_R3_Pin|KEYPAD_R2_Pin|KEYPAD_R1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_LED_Pin */
  GPIO_InitStruct.Pin = LCD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEYPAD_C3_Pin */
  GPIO_InitStruct.Pin = KEYPAD_C3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEYPAD_C3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
