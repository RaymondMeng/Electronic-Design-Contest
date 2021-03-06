/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MLX90614.h"
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char buf[30];
uint8_t len;

float ObjectTemperature;
float AmbientTemperature;
float Emissivity;
uint8_t Temperature;
uint8_t UART1_RxBuffer;
uint8_t UART3_RxBuffer;
uint8_t buffer1[7] = "\0";
uint8_t buffer2[7] = "\0";
uint8_t mode = 0; //默认0:模式0  1：模式1
uint8_t num, score, state;
uint8_t end[3]= {0xff,0xff,0xff};
uint16_t dist;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  串口发送函数
  * @retval None
  */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define    TXBUF_SIZE_MAX    100

void uartx_printf(UART_HandleTypeDef huartx, const char *format, ...)
{
    va_list args;
    uint32_t length;
    uint8_t txbuf[TXBUF_SIZE_MAX] = {0};
 
    va_start(args, format);
    length = vsnprintf((char *)txbuf, sizeof(txbuf), (char *)format, args);
    va_end(args);
    HAL_UART_Transmit(&huartx, (uint8_t *)txbuf, length, HAL_MAX_DELAY);
    memset(txbuf, 0, TXBUF_SIZE_MAX);
};
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  MLX90614_Init(&hi2c1);
  MLX90614_SetEmissivity(0.985); // Human skin
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1); //开启串口中断
  HAL_UART_Receive_IT(&huart3, (uint8_t*)&UART3_RxBuffer, 1); //开启串口中断
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    MLX90614_ReadAmbientTemperature(&AmbientTemperature);
    MLX90614_ReadObjectTemperature(&ObjectTemperature);
    MLX90614_GetEmissivity(&Emissivity);
    
//    memset(buf, 0x00, 30); //清空数组
//	  len = sprintf(buf, "Emissivity: %.3f\n\r", Emissivity);   //辐射率
	  //HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 20);

//	  memset(buf, 0x00, 30);
//	  len = sprintf(buf, "Ambient: %.2f\n\r", AmbientTemperature);    //室温
	  //HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 20);

	  memset(buf, 0x00, 30);
	  len = sprintf(buf, "Object: %.2f\n\n\r", ObjectTemperature);    //目标温度
	  //HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, 20);
	  HAL_Delay(50);
    
    if(!mode){
      score = buffer1[2];
      num = buffer1[1]; //0代表未识别的人 1，2，3分别代表三个人
      //uartx_printf(huart2, "num: %d  score: %d\r\n", num, score);
//      uartx_printf(huart2, "")
    }
    else
    {
      state = buffer1[1]; //口罩识别状态
      score = buffer1[2]; //置信率
      //uartx_printf(huart2, "state: %d score: %d\r\n", state, score);
    }
    uartx_printf(huart2, "n0.val=%d", dist);
    HAL_UART_Transmit(&huart2,end,3,0xffff);
    
    Temperature = (uint8_t)ObjectTemperature;
    uartx_printf(huart2, "n1.val=%d", Temperature);
    HAL_UART_Transmit(&huart2,end,3,0xffff);
    HAL_Delay(20);
    if(state == 0){
      uartx_printf(huart2, "t8.txt=\"请您佩戴口罩\"");
      HAL_UART_Transmit(&huart2,end,3,0xffff);
    }
    else if(state && Temperature<37.3)
    {
      uartx_printf(huart2, "t8.txt=\"体温正常\"");
      HAL_UART_Transmit(&huart2,end,3,0xffff);
    }
    else if(state && Temperature>=37.3)
    {
      uartx_printf(huart2, "t8.txt=\"体温异常\"");
      HAL_UART_Transmit(&huart2,end,3,0xffff);
    }
    HAL_Delay(20);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   static uint8_t count = 0;
   static uint8_t count1 = 0;
   if(huart->Instance == huart1.Instance)
   {
     if(count == 0 && UART1_RxBuffer == 0xff)
     {
        count = 1;
        buffer1[0] = 0xff;
     }
     else if(count == 1)
     {
        count = 2;
        buffer1[1] = UART1_RxBuffer;
     }
     else if(count == 2)
     {
        count = 3;
        buffer1[2] = UART1_RxBuffer;
     }
     else if(count == 3 && UART1_RxBuffer == 0xfe)
     {
        count = 0;
        buffer1[3] = 0xfe;
     }
     else
       count = 0;
     HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
   }
   
   if(huart->Instance == huart3.Instance)
   {
      if(count1 == 0 && UART3_RxBuffer == 0x59)
      {
       count1++;
       buffer2[0] = UART3_RxBuffer;
      }
      else if(count1 == 1 && UART3_RxBuffer == 0x59)
      {
         count1++;
         buffer2[1] = UART3_RxBuffer;
      }
      else if(count1 == 2)
      {
         count1++;
         buffer2[2] = UART3_RxBuffer;
      }
      else if(count1 == 3)
      {
         buffer2[3] = UART3_RxBuffer;
         dist = buffer2[3]<<8 | buffer2[2];
         count1 = 0;
      }
      else
        count1 = 0;
      HAL_UART_Receive_IT(&huart3, (uint8_t*)&UART3_RxBuffer, 1);
   }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(GPIO_Pin == key_Pin) {
      HAL_GPIO_WritePin(mode_GPIO_Port, mode_Pin, GPIO_PIN_SET);
      //uartx_printf(huart2, "hello_world!\r\n");
      mode = 1;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
