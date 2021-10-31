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
#include "servo.h"
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
uint8_t UART1_RxBuffer, UART2_RxBuffer;
uint8_t buffer1[9] = "\0", buffer2[10] = "\0";
uint8_t Target_Flag = 0;
uint8_t Real_x, Real_y;
uint8_t color;
uint8_t length;
uint8_t shape;
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
  MX_TIM3_Init();
  MX_TIM9_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1); //开启定时器中断
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //开启pwm
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1); //开启pwm
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1); //开启串口中断,用于openmv
  HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART2_RxBuffer, 1); //开启串口2中断用于激光测距
  
  PID_Init();
  Servo_Init(); //云台初始化，两自由度均为90°
  PID_Init(); //PID初始化
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
//    if(Target_Flag){
//      Tim_SetPWM(3, 1, positionx_pid.output); 
//      Tim_SetPWM(9, 1, positiony_pid.output);
//    }
    //DBG("hello!");
    //DBG("shape:%d length:%d color:%d x:%d y:%d", shape, length, color, Real_x, Real_y);
      uartx_printf(huart2, "t2.txt=\"%d\"", dist);
      HAL_UART_Transmit(&huart2,end,3,0xffff);
      uartx_printf(huart2, "t7.txt=\"%d\"", shape);
      HAL_UART_Transmit(&huart2,end,3,0xffff);
      uartx_printf(huart2, "t8.txt=\"%d\"", length);
      HAL_UART_Transmit(&huart2,end,3,0xffff);
      uartx_printf(huart3, "hellworld\r\n");
      
//    HAL_Delay(1000);
//    Tim_SetPWM(3, 1, 2340);
//    Tim_SetPWM(9, 1, 1500);    
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
  __HAL_UART_CLEAR_PEFLAG(huart);
  if(huart -> Instance == huart1.Instance) //如果是串口1
  {
   // DBG("hello!");
      if((count == 0) && (UART1_RxBuffer == 0xff))
      {    
        count = 1;
        buffer1[0] = UART1_RxBuffer;
      }
      else if(count == 1)
      {
        buffer1[1] = UART1_RxBuffer;
        if(UART1_RxBuffer == 0) //没有检测到目标
          Target_Flag = 0; 
        else
        {
          count = 2;               //有才加1
          Target_Flag = 1;
        }
      }
      else if(count == 2)
      {
        buffer1[2] = UART1_RxBuffer;
        count = 3;
      }
      else if(count == 3)
      {
          buffer1[3] = UART1_RxBuffer;
          count = 4;
      }
      else if(count == 4)
      {
          buffer1[4] = UART1_RxBuffer;
          count = 5;
      }
      else if(count == 5)
      {
          buffer1[5] = UART1_RxBuffer;
          count = 6;
      }
      else if(count == 6)
      {
          buffer1[6] = UART1_RxBuffer;
          count = 7;
      }
      else if((count == 7) && (UART1_RxBuffer == 0xfe))
      {
          count = 0;
          buffer1[7] = UART1_RxBuffer;
          Real_x = buffer1[2];
          Real_y = buffer1[3];
          color = buffer1[6];
          length = buffer1[4]<<8 | buffer1[5]; //物体边长的像素长度
          Target_Flag = 1;
          shape = buffer1[1];
          //HAL_UART_Transmit(&huart2, buffer1, 7, HAL_MAX_DELAY);
      }
      else{
         count = 0;
         Target_Flag =0;
      }
//        //加个数据处理
     //uartx_printf(huart2, "%x %x %x %x %x\r\n", 0xff, Real_x, Real_y, buffer1[5], 0xfe);
      HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1); //重新开启中断
   } 
   uartx_printf(huart3, "here\r\n");
   
  /*串口2中断用于接收激光测距值*/
   if(huart -> Instance == huart2.Instance)
   {
     uartx_printf(huart3, "there\r\n");
     if(count1 == 0 && UART2_RxBuffer == 0x59)
     {
       count1 = 1;
       buffer2[0] = UART2_RxBuffer;
     }
     else if(count1 == 1 && UART2_RxBuffer == 0x59)
     {
       count1 = 2;
       buffer2[1] = UART2_RxBuffer;
     }
    else if(count1 == 2)
    {
       count1 = 3;
       buffer2[2] = UART2_RxBuffer;
    }
    else if(count1 == 3)
    {
       buffer2[3] = UART2_RxBuffer;
       count1 = 0;
       dist = buffer2[3]<<8 | buffer2[2];
    }
    else
       count1 = 0;
  }
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1); //重新开启中断
  HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART2_RxBuffer, 1);
}

/**
  * @brief  定时器中断函数
  * @claim  10ms为周期
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
   //频率100Hz
  //DBG("have received successfully!\r\n");
  //DBG("have received successfully!\r\n");
//  uint32_t output_temp;
  if (htim->Instance == htim1.Instance){
    positionx_pid.measured_value =  Real_x;
    positionx_pid.error          =  positionx_pid.measured_value - positionx_pid.set_value;
    positionx_pid.integral       =  positionx_pid.integral + positionx_pid.error * 0.01; //dt为采样频率
    positionx_pid.derival        =  (positionx_pid.error - positionx_pid.passive_error) / 0.01; //dt为采样频率（用定时器中断）
    positionx_pid.output         =  current_pulses_x - (positionx_pid.kp * positionx_pid.error + positionx_pid.ki * positionx_pid.integral + positionx_pid.kd * positionx_pid.derival);

    //    positionx_pid.output         =  output_temp > 1500 ? 1500 : output_temp;
//    positionx_pid.output         =  output_temp <  500 ?  500 : output_temp;
    positionx_pid.passive_error  =  positionx_pid.error;
    //DBG("have received successfully!\r\n"); 
    positiony_pid.measured_value =  Real_y;
    positiony_pid.error          =  positiony_pid.measured_value - positiony_pid.set_value;
    positiony_pid.integral       =  positiony_pid.integral + positiony_pid.error * 0.01; //dt为采样频率
    positiony_pid.derival        =  (positiony_pid.error - positiony_pid.passive_error) / 0.01; //dt为采样频率（用定时器中断）
    positiony_pid.output         =  current_pulses_y - (positiony_pid.kp * positiony_pid.error + positiony_pid.ki * positiony_pid.integral + positiony_pid.kd * positiony_pid.derival) ;

    //    positionx_pid.output         =  output_temp > 1500 ? 1500 : output_temp;
//    positionx_pid.output         =  output_temp <  500 ?  500 : output_temp;
    positiony_pid.passive_error  =  positiony_pid.error;
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
