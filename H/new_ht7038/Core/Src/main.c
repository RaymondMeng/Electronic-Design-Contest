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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define		TEST_BUF_SIZE	50		//测试缓存大小
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t 	ReceData[3]= {0};
uint16_t	count = 0;						//计数器
uint16_t	data  = 0;
//保存原始电流有效值
 uint32_t	A_Original_Buf[TEST_BUF_SIZE]={0};	//A相原始电流有效值
 uint32_t	B_Original_Buf[TEST_BUF_SIZE]={0};	//B相原始电流有效值
 uint32_t	C_Original_Buf[TEST_BUF_SIZE]={0};	//C相原始电流有效值

//保存转换后的电流有效值
 uint16_t a_changed_buf[TEST_BUF_SIZE] = {0};	//A相转换后的电流值
 uint16_t b_changed_buf[TEST_BUF_SIZE] = {0};	//B相转换后的电流值
 uint16_t c_changed_buf[TEST_BUF_SIZE] = {0};	//C相转换后的电流值
 
uint8_t	APhaseReceBuf[3] = {0};
uint8_t	BPhaseReceBuf[3] = {0};	
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "stdio.h"
int fputc(int ch, FILE *f){
uint8_t temp[1] = {ch};
HAL_UART_Transmit(&huart1, temp, 1, 2);//huart1需要根据你的配置修改
return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint16_t		i = 0;
	float	temp = 0.0;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HT7038ResetSet();
  SetHT7038Function();
  printf("\r\n\r\nht7038模块开始测试！\r\n\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if( count < 20 )
		{
			//读取A相电流有效值
			ReadSpi(R_IARMS&0x7f, APhaseReceBuf);
			//读取B相电有效值//R_IBRMS
			ReadSpi(R_UARMS&0x7f, BPhaseReceBuf);
			
			A_Original_Buf[count] = (APhaseReceBuf[0]<<16) | (APhaseReceBuf[1]<<8) | (APhaseReceBuf[2]);
			//a_changed_buf[count] = (u16) (A_Rece_Buf[count]/8192/RES_VALUE*10);
			temp = (A_Original_Buf[count]/8192/RES_VALUE*10);
			a_changed_buf[count] = (uint16_t)temp;

			temp = 0.0;

			B_Original_Buf[count] = (BPhaseReceBuf[0]<<16) | (BPhaseReceBuf[1]<<8) | (BPhaseReceBuf[2]);
			//b_changed_buf[count] = (u16) (B_Rece_Buf[count]/8192/RES_VALUE*10);
			temp = (B_Original_Buf[count]/8192/RES_VALUE*10);
			b_changed_buf[count] = (uint16_t)temp;
			count++;			
		}
		else
		{
			printf("A相原始的电流有效值为：\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %2d  ",A_Original_Buf[i]);
			}
			printf("\r\n");
			printf("A相转换后的电流有效值为：\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %d  ",a_changed_buf[i]);
			}
			
			printf("\r\n");
			
//			printf("B相原始的电压有效值为：\r\n");
//			for(i=0;i<count;i++)
//			{
//				printf(" %2x  ",B_Original_Buf[i]);
//			}
//			printf("\r\n");
//			printf("B相转换后的电压有效值为：\r\n");
//			for(i=0;i<count;i++)
//			{
//				printf(" %d  ",b_changed_buf[i]);
//			}
			
			count = 0;
			printf("\r\n******************************************************\r\n");
			printf("\r\n");
		}
		HAL_Delay(20);	//20ms读取一次
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
