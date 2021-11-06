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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ht7038.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CURRENT_DAT_TIMES 256
#define DATA_RATIO 94
#define TYPE_NUM 6
#define FEATURE_NUM 5
#define buff_SIZE 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//A相电流有效值 A相有功功率 A相无功功率 A相功率因数 A相电流与电压相角
int feature_flash[TYPE_NUM][FEATURE_NUM] = {
     500,    5,        27,      2348810,   -524472,   //没有插任何电器
     8000,   649,      800,     5321192,   397179,   //灯带
     11174,  -1440,    4197,    -2739561,  637365,   //电风扇
     2245,   120,      792,     1288176,   371386,     //节能灯
     2891,   19,       367,     490698,    450987,      //树莓派
     799341, 108485,   319306,  2695306,   366848,       //热水壶
      
};
float distance[TYPE_NUM] = {0};   
float temp_long=0;
int HT7038[FEATURE_NUM] = {0};
float temp_f32 = 0.0;
uint32_t temp_u32 = 0;
uint32_t class_array[buff_SIZE] = {0};
uint32_t class_keep = 0;
unsigned int temp;
uint32_t count;

int tx_buf[10] = {0};
int cnt;

uint8_t end[3]= {0xff,0xff,0xff};
uint8_t UART2_RxBuffer;
uint8_t mode = 0;

uint8_t yougong_p, wugong_p;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void CurrentDataReadFun(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "stdio.h"
int fputc(int ch, FILE *f){
uint8_t temp[1] = {ch};
HAL_UART_Transmit(&huart1, temp, 1, 2);//huart1需要根据你的配置修改

return ch;
}

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
     int i, j;
    
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
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART2_RxBuffer, 1); //开启串口中断
	att7053_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //temp = att7053_Read(0x10);
    temp_u32 = 0;
	  temp_f32 = 0.0;
    for(cnt=0;cnt<TYPE_NUM;cnt++)
        distance[cnt]=0;
    CurrentDataReadFun();
    
    //计算当前特征与flash特征距离
    for(i = 0;i<TYPE_NUM;i++){
      for(j = 0;j<FEATURE_NUM;j++){
        if(j==3)
            temp_f32 = pow(feature_flash[i][j]/10000 - HT7038[j]/10000,2);
        else
            temp_f32 = pow(feature_flash[i][j]/1000 - HT7038[j]/1000,2);
        distance[i] += temp_f32;
        printf("%f ", temp_f32);
      }
      printf("dist:%f \r\n ",distance[i]);
    }
    printf("\r\n");
    
      //寻找距离最小的类别
      temp_f32 = distance[0];
      for(i = 0;i<TYPE_NUM;i++){
        if(distance[i] < temp_f32){
          temp_f32 = distance[i];
          temp_u32 = i;
        }
      }
      
	  //发送最小的类别

      j = 0;
      //判断数组里是否都是一样的 更新keep数组
      for(i=0;i<buff_SIZE-1;i++){
        if(class_array[i] != class_array[i+1])
             j++;
        class_array[i] = class_array[i+1];
      }
      class_array[buff_SIZE-1] = temp_u32;
      if(j==0)
        class_keep = temp_u32;
      printf("class: %d\r\n",class_keep);
      
      uartx_printf(huart2, "t0.txt=\"用电器%d\"", class_keep);
      HAL_UART_Transmit(&huart2,end,3,0xffff);
      
      uartx_printf(huart2, "n1.val=%d", tx_buf[2]);
      HAL_UART_Transmit(&huart2,end,3,0xffff);
      
      uartx_printf(huart2, "n2.val=%d", tx_buf[6]);
      HAL_UART_Transmit(&huart2,end,3,0xffff);
      
      if(mode)//学习模式
      {
          memset(feature_flash, 0x00, sizeof(feature_flash));
          HAL_Delay(100); //等待稳定后开始测量
          for(i = 0; i < 10; i++)
          {
            
          }
          mode = 0;
      }
          
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(800);
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
void CurrentDataReadFun()
{
  int i = 0;
  int temp = 0;

  tx_buf[0] = 0xff;
	tx_buf[1] = att7053_Read(0x0D); //A相电压
  
	//currentDataBuf[1] += att7053_Read(0x12); //C相电流有效值
	tx_buf[2] = att7053_Read(0x10); //A相电流有效值
  HT7038[0] = tx_buf[2];
	//currentDataBuf[5] += att7053_Read(0x10); //A相电流有效值
  temp = att7053_Read(0x01); //A相有功功率
  if(temp>8388608) tx_buf[3] = temp - 16777216;
  else tx_buf[3] = temp;
  HT7038[1] = tx_buf[3];
  
  temp = att7053_Read(0x05); //A相无功功率
  if(temp>8388608) tx_buf[4] = temp - 16777216;
  else tx_buf[4] = temp;
  HT7038[2] = tx_buf[4];
  
  //tx_buf[5] = att7053_Read(POWERP1); //B相视在功率
  temp = att7053_Read(0x14); //A相功率因数
  if(temp>8388608) tx_buf[5] = temp - 16777216;
  else tx_buf[5] = temp;
  HT7038[3] = tx_buf[5];
  
  temp = att7053_Read(0x18);  //A相电流与电压相角
  if(temp>1048576) tx_buf[6] = temp - 16777216;
  else tx_buf[6] = temp;
  HT7038[4] = tx_buf[6];
  
  tx_buf[7] = 0xfe;


  printf("\r\n1.%d\r\n", tx_buf[2]);
  printf("\r\n2.%d\r\n", tx_buf[3]);
  printf("\r\n3.%d\r\n", tx_buf[4]);
  printf("\r\n4.%d\r\n", tx_buf[5]);
  printf("\r\n5.%d\r\n", tx_buf[6]);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if(huart->Instance == huart2.Instance)
   {
      if(UART2_RxBuffer == 0x01)
      {
          uartx_printf(huart2, "t8.txt=\"学习中...\"");
          HAL_UART_Transmit(&huart2,end,3,0xffff);
          mode = 1; //模式切换
      }
      HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART2_RxBuffer, 1);      
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
