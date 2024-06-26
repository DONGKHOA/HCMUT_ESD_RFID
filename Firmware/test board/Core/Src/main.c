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
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
state_handle_t state_task = INIT_STATE;
state_door_t state_door;
uint8_t data_keypad[15];
uint8_t data_temp;
uint8_t data = 13;
static uint8_t state_no_null;
static uint8_t change_state_null = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void STATE_BUTTON_Handle(void)
{
  if (data == 13)
  {
    if (state_no_null == 1)
    {
      change_state_null = 1;
      state_no_null = 0;
    }
  }
  else
  {
    state_no_null = 1;
  }
}

uint8_t ENTER_Password(uint8_t *data_keypad)
{
  static uint8_t dest = 0;

  data = (uint8_t)keypad_handle();
  if ((data != 13) &&  (change_state_null == 1))
  {
    if ((data == '#') || (data == '*'))
    {
      dest = 0;
      return data;
    }
    *(data_keypad + dest) = data;
    dest = dest + 1;
    change_state_null = 0;
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
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
