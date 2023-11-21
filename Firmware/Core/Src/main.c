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
#include "rc522.h"
#include "memory.h"
#include "string.h"
#include "key_pad_basic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYPAD_STATE            0

#define MODE_ADMIN              1
  #define ADMINISTRATOR
    #define CHANGE_PASSWORD_ADMIN   2
    #define UNBLOCK_USER            3
  #define CHANGE_PASSWORD_USER    4
  #define ADD_NEW_CARD            5
#define MODE_USER               6

#define BLOCK_SYSTEM            7
#define BLOCK_USER              8
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */
uint8_t status;
u_char str[MAX_LEN]; // Max_LEN = 16
uint8_t serNum[5];
uint16_t flag = 0;
uint8_t block_system = 0;
uint8_t _index = 0;

//Using key pad
uint8_t password[15];
uint8_t admin_password_store[15] = "*#000#12345678";
uint8_t user_password_store[10] = "123456789";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SET_bitmask(uint16_t* group, uint8_t bit)
{
  *group |= (1 << bit);
}

void CLEAR_bitmask(uint16_t* group, uint8_t bit)
{
  *group &= ~(1 << bit);
}

void __CHANGE_Password_Admin(void)
{
	uint8_t temp = 0;
  _index = 6;
  for(uint8_t i = 0; i < 6; i++)
  {
    password[i] = admin_password_store[i];
  }
  // hiện thị yêu cầu mật khẩu admin có 8 kí tự
  if ((flag & (1 << CHANGE_PASSWORD_ADMIN)) == (1 << CHANGE_PASSWORD_ADMIN))
  {    
    while (1)
    {
      temp = keypad_handle();
      if(temp != NONE_PRESSING_STATE)
      {
            password[_index] = temp;
            if(_index == 14)
            {
              ClearBitMask(&flag, CHANGE_PASSWORD_ADMIN);
              _index = 0;
              break;
            }
            _index++;
      }
    }
  }
  //hiên thị nhấn # để xác nhận rồi quay lại màn hình lựa ch�?n của admin
  //hoặc nhấn * để quay lại (màn hình đổi mật khẩu)
  while(1)
  {
    temp = keypad_handle();
    if(temp == '#')
    {
      memcpy(admin_password_store, password, sizeof(admin_password_store));
      break;
    }
    else if(temp = '*')
    {
      SetBitMask(flag, CHANGE_PASSWORD_ADMIN);
      break;
    }
  }
}

void __UNBLOCK_User(void)
{
  if((flag & (1 <<UNBLOCK_USER)) == (1 << UNBLOCK_USER))
  {
    // nhấn # để unblock user rồi quay lại màn hình lựa ch�?n của admin
    // nhấn * rồi quay lại màn hình quản trị viên (administrator)
    uint8_t temp = 0;
    while(1)
    {
      temp = keypad_handle();
      if(temp == '#')
      {
        block_system = 0;
      }
      else if(temp == '*')
      {
        ClearBitMask(flag, UNBLOCK_USER);
      }
    }
  }
}

void _administrator(void)
{
	// hiện thị lựa ch�?n
  //1.đổi mật khẩu admin
  //2. mở khóa user
  uint8_t selection = 0;
  selection = keypad_handle();
  if(selection == '1')
  {
    SetBitMask(flag, CHANGE_PASSWORD_ADMIN);
    __CHANGE_Password_Admin();
  }
  else if(selection == '2')
  {
    SetBitMask(flag, CHANGE_PASSWORD_ADMIN);
  }
}

void admin_mode(uint8_t choice)
{
  // hiển thị ch�?n mode trong admin mode
  //1.change password admin
  //2.Unblock user
  //3. thêm / bớt thẻ
	  switch(choice)
	  {
	  case '1':
      _administrator();
		  break;
	  case '2':
		  break;
	  case '3':
		  break;
	  }
}

void user_mode(void)
{
  // uint8_t block_user = 0;
  // char re_user_password_store[10];
  // uint8_t i = 0;
  // while (1) 
  // {
  //   if(block_system < 5)
	//   {
  //     if ((flag & (1 << KEYPAD_STATE)) != (1 << KEYPAD_STATE))
  //     {        
  //       while(keypad_handle() != NONE_PRESSING_STATE)
  //       {
  //         password[i] = keypad_handle();
  //         i++;
  //         if(i == 14)
  //         {
  //           flag |= (1 << KEYPAD_STATE);
  //           i = 0;
  //         }
  //       }
  //     }
        
  //     if((flag & (1 << KEYPAD_STATE)) == (1 << KEYPAD_STATE))
  //     {
  //       uint8_t temp = re_user_password_store[14];
  //       re_user_password_store[14] = '\0';
  //       if(strcmp(re_user_password_store, user_password_store) == 0)
  //       {
  //         // display successful unlocking
  //         // turn on relay
  //       }
  //       else
  //       {
  //         block_user++;
  //       }
  //     }
  //   }
  //   else        // user_mode blocked
  //   {
  //     // block user input password!!!!
  //   }
  // }

  ///////turn on relay
}
void card_mode(void)
{

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
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(block_system < 5)
	  {
      if ((flag & (1 << KEYPAD_STATE)) != (1 << KEYPAD_STATE))
      {        
        while(1)
        {
          uint8_t temp = keypad_handle();
          if(temp != NONE_PRESSING_STATE)
          {
            password[_index] = temp;
            if(_index == 14)
            {
              SetBitMask(&flag, KEYPAD_STATE);
              _index = 0;
              break;
            }
            _index++;
          }
        }
      }
      
		  if((flag & (1 << KEYPAD_STATE)) == (1 << KEYPAD_STATE))
		  {
			  uint8_t temp = password[14];
			  password[14] = '\0';
			  if(strcmp(password, admin_password_store) == 0)
			  {
				  admin_mode(temp);

			  }
			  else if (strcmp(password, user_password_store) == 0)
			  {

			  }
			  else
			  {
				  block_system++;
			  }
		  }
	  }
	  else
	  {
		  // store state block system in eeprom
      SetBitMask(flag, BLOCK_USER);
	  }
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
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RS_Pin */
  GPIO_InitStruct.Pin = RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS_GPIO_Port, &GPIO_InitStruct);

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
