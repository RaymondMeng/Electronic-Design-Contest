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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
const float PreCalcu_Data[] = {
 0.000, 
0.035, 0.070, 0.104, 0.139, 0.172, 0.206, 0.239, 0.271, 0.302, 0.333,
0.363, 0.392, 0.420, 0.447, 0.474, 0.499, 0.524, 0.547, 0.570, 0.592,
0.613, 0.633, 0.652, 0.671, 0.689, 0.706, 0.722, 0.738, 0.753, 0.767,
0.781, 0.794, 0.806, 0.818, 0.830, 0.841, 0.851, 0.861, 0.871, 0.880,
0.889, 0.897, 0.905, 0.913, 0.920, 0.927, 0.934, 0.941, 0.947, 0.953,
0.958, 0.964, 0.969, 0.974, 0.978, 0.983, 0.987, 0.991, 0.995, 0.999,
1.002, 1.006, 1.009, 1.012, 1.015, 1.018, 1.020, 1.023, 1.025, 1.028,
1.030, 1.032, 1.034, 1.035, 1.037, 1.039, 1.040, 1.042, 1.043, 1.044,
1.045, 1.046, 1.047, 1.048, 1.049, 1.050, 1.051, 1.051, 1.052, 1.052,
1.053, 1.053, 1.054, 1.054, 1.054, 1.054, 1.054, 1.054, 1.054, 1.054
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#include "stdio.h"
/**
  * @brief  串口调试
  * @retval None
  */
#define DEBUG
#ifdef DEBUG
#define DBG(format, ...) fprintf(stdout, "[\tDBG](File:%s, Func:%s(), Line:%d): " \
                                , __FILE__, __FUNCTION__, __LINE__);             \
                         fprintf(stdout, format"\r\n", ##__VA_ARGS__)
#else
#define DBG(format, ...)  do {} while (0)
#endif

int fputc(int ch, FILE *f){
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart1, temp, 1, 2);//huart1 根据配置修改
    return ch;
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t UART1_RxBuffer;
uint8_t buffer1[8] = "\0";
uint16_t Real = 1500;
uint8_t mode = 0x01;
uint8_t key = 1;
uint8_t count;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启PWM
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 1500);
  HAL_Delay(1000);
  //
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    if(key == 4)
      key = 0;
    mode = key;
    if(mode == 0x01){ //模式一
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Real);
      DBG("hello1");
    }
    
//    else if(mode == 0x02){  //模式三
//      DBG("hello2");
//      if(Real-1500 < 0)
//        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500 - after_change_pulses); 
//      if(Real-1500 > 0)
//        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500 + after_change_pulses);
//    }
    
    else if(mode == 0x03) { //用预处理数据
      DBG("hello3");
      if(Real-1500 < 0){
        count = (int)(xita/jiange);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500 - (PreCalcu_Data[count] / Pi * 2000 + chazhi)); 
      }
      if(Real-1500 > 0){
        count = (int)(xita/jiange);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500 + (PreCalcu_Data[count] / Pi * 2000 + chazhi));
      }
    }
    
//    else if(mode == 0x04)  //提高部分
//       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, after_change_pulses + 1500);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  static uint8_t count = 0;
  if(huart -> Instance == USART1)
  {
    if((count == 0) && (UART1_RxBuffer == 0xff))
    {
      count++;
      buffer1[0] = UART1_RxBuffer;
    }
    else if((count == 1) && (UART1_RxBuffer < 0xff))
    {
      buffer1[1] = UART1_RxBuffer;
      count++;
    }
    else if((count == 2) && (UART1_RxBuffer <= 0xff))
    {
      buffer1[2] = UART1_RxBuffer;
      count++;
    }
    else if((count == 3) && (UART1_RxBuffer == 0xfe))
    {
      count = 0;
      buffer1[3] = UART1_RxBuffer;
      Real = (buffer1[1] << 8) | buffer1[2];
      //DBG("hello!\r\n");
    }
    else
       count = 0;
    HAL_UART_Receive_IT(&huart1, &UART1_RxBuffer, 1);
  }
}

/**
  * @brief  GPIO外部中断
  * @param  GPIO_Pin
  * @retval None
  * @claim  按键外部中断程序
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(GPIO_Pin == GPIO_PIN_10) {
    key++;
    while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10));  
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
