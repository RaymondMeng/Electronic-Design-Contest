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
#define TYPE_NUM 8
#define FEATURE_NUM 5
#define buff_SIZE 32
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//A相电流有效值 A相有功功率 A相无功功率 A相功率因数 A相电流与电压相角
int feature_flash[TYPE_NUM][FEATURE_NUM] = {
     1082,    9,        26,      0,   -524344,   //没有插任何电器
     7555,   636,      265,     0,   0,   //灯带                   1         1
     10235,  -1476,    4110,    0,  0,   //电风扇                  2         2
     1320,   122,      774,     0,   0,     //节能灯               4         3
     2254,   12,       368,     0,    0,      //树莓派             8         4
     528,    154,       450,   0,     0,                   //盖佬的电阻        16
     1749,    338 ,     453,   0,     0,   //自制1号                                32
     860000, 115234, 343953,   0,   0,       //电热器             64
      
};


//有关计算类别的变量
/********************************/
uint32_t count;
float distance = 0; 
int class_min;
int HT7038[FEATURE_NUM] = {0};
//有功 无功
int CalcFeature[4] = {0};
float temp_f32 = 0.0; 
uint32_t temp_u32 = 0; 
long int temp_long = 0; 

//均值
int Feature_Buffer[3][buff_SIZE] = {0};
int average[4] = {0};
uint32_t buff_count = 0;
/********************************/

unsigned int temp;


int cnt;

int tx_buf[10] = {0};
int cnt;

uint8_t end[3]= {0xff,0xff,0xff};
uint8_t UART2_RxBuffer;
uint8_t mode = 0;
uint8_t mat_cnt = 0;

uint8_t yougong_p, yougong_sum, yougong_avg, yougong_final, yougong_s;

uint8_t wugong_p, wugong_sum, wugong_avg, wugong_final, wugong_s;

uint8_t youxiao_c, youxiao_sum, youxiao_avg, youxiao_final, youxiao_s;

uint8_t yinshu, yinshu_sum, yinshu_avg, yinshu_final, yinshu_s;

uint8_t xiangjiao, xiangjiao_sum, xiangjiao_avg, xiangjiao_final, xiangjiao_s;
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
      temp_u32 = 0;
      temp_f32 = 0.0;
    
      //读取特征量到HT7038
      CurrentDataReadFun();
        
      //有功无功均值
      for(i = 0;i < 3;i++){
          temp_long = 0;
          for(j = 0; j < buff_SIZE;j++){
              temp_long += Feature_Buffer[i][j];
          }
          temp_u32 = temp_long/buff_SIZE;
          average[i] = temp_u32;
          printf("%d ",temp_u32);
      }
      printf("\r\n");
      //识别模式
      if(!mode){

          //初始化参数
          distance = pow(average[0]/10,2) + pow(average[1]/10,2); //+ pow(average[2]/10,2);
          //printf("0: %f ",distance);
          class_min = 0;     
          //根据有功无功建立叠加表
          for(i = 1;i < (1 <<(TYPE_NUM-1));i++){
              CalcFeature[0] =
                  feature_flash[1][1] * (i 	   &0x01)   +
                  feature_flash[2][1] * ((i>>1)&0x01) +
                  feature_flash[3][1] * ((i>>2)&0x01) +
                  feature_flash[4][1] * ((i>>3)&0x01) +
                  feature_flash[5][1] * ((i>>4)&0x01) +
                  feature_flash[6][1] * ((i>>5)&0x01) +
                  feature_flash[7][1] * ((i>>6)&0x01) ;
            
              CalcFeature[1] =
                  feature_flash[1][2] * (i 	 &0x01)   +
                  feature_flash[2][2] * ((i>>1)&0x01) +
                  feature_flash[3][2] * ((i>>2)&0x01) +
                  feature_flash[4][2] * ((i>>3)&0x01) +
                  feature_flash[5][2] * ((i>>4)&0x01) +
                  feature_flash[6][2] * ((i>>5)&0x01) +
                  feature_flash[7][2] * ((i>>6)&0x01) ;
            
              CalcFeature[2] =
                  feature_flash[1][2] * (i 	 &0x01)   +
                  feature_flash[2][2] * ((i>>1)&0x01) +
                  feature_flash[3][2] * ((i>>2)&0x01) +
                  feature_flash[4][2] * ((i>>3)&0x01) +
                  feature_flash[5][2] * ((i>>4)&0x01) +
                  feature_flash[6][2] * ((i>>5)&0x01) +
                  feature_flash[7][2] * ((i>>6)&0x01) ;
          //最小欧拉距离
              temp_f32 = pow(CalcFeature[0]/10 - average[0]/10,2) + pow(CalcFeature[1]/10 - average[1]/10,2); //+ pow(CalcFeature[2]/10 - average[2]/10,2);
              //printf("%d: %d %d %f ",i,CalcFeature[0],CalcFeature[1],temp_f32);
              if(temp_f32 < distance){
                  distance = temp_f32;
//                  if(average[0]>110000 && average[1]>340000)
//                    i = 7;
                  class_min = i;
                  if(average[2] > 780000 && average[2] < 800000)
                    class_min = 64;
              }
          }

          printf("class: %d\r\n",class_min);
          printf("phase: %f\r\n",(float)HT7038[4]*0.000171661);
          printf("line freq: %f\r\n",(float)HT7038[3]/8192.0);
          
          //uartx_printf(huart2, "t0.txt=\"用电器%d\"", class_min);
          //1234567
          uartx_printf(huart2, "t0.txt=\"");
          for(i = 1;i < TYPE_NUM;i++){
              if(class_min&(0x01<<(i-1))){
                  uartx_printf(huart2, "用电器%d ", i);        
              }             
          }
          uartx_printf(huart2, "\"", i);
          HAL_UART_Transmit(&huart2,end,3,0xffff);
          
          uartx_printf(huart2, "n1.val=%d", tx_buf[2]);
          HAL_UART_Transmit(&huart2,end,3,0xffff);
          
          uartx_printf(huart2, "n2.val=%d", (int)HT7038[4]*0.000171661);
          HAL_UART_Transmit(&huart2,end,3,0xffff);
      }
      
      
      /*学习模式*/
      if(mode)
      {
          if(mat_cnt==0){
            //第0行是底噪  不清空
              feature_flash[mat_cnt][1] += average[0];
              feature_flash[mat_cnt][2] += average[1];
            //清空
              for(i = 1;i < TYPE_NUM;i++){
                  for(j = 0; j < FEATURE_NUM;j++){
                      feature_flash[i][j] = 0;
                  }
              }              
          }
          else{
            //替换
              feature_flash[mat_cnt][1] = average[0];
              feature_flash[mat_cnt][2] = average[1];
          }

          mat_cnt++;
          if(mat_cnt>=TYPE_NUM)
                mat_cnt = 0;
        
          uartx_printf(huart2, "t8.txt=\"学习完成\"");
          HAL_UART_Transmit(&huart2,end,3,0xffff);
          printf("学习完成\r\n");
          
          //打印出来看
          for(i = 0;i < TYPE_NUM;i++){
              for(j = 0; j < FEATURE_NUM;j++){
                  printf("%d ",feature_flash[i][j]);
              }
              printf("\r\n");
          }
          
          mode = 0;
          HAL_Delay(2000);
      }
          
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(80);
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
  int temp = 0;

  tx_buf[0] = 0xff;
	tx_buf[1] = att7053_Read(0x0D); //A相电压
  

	tx_buf[2] = att7053_Read(0x10); //A相电流有效值
  HT7038[0] = tx_buf[2] - feature_flash[0][0];  //减去电流偏置

  temp = att7053_Read(0x01); //A相有功功率
  if(temp>8388608) tx_buf[3] = temp - 16777216;
  else tx_buf[3] = temp;
  HT7038[1] = tx_buf[3] - feature_flash[0][1];  //减去有功偏置
  
  temp = att7053_Read(0x05); //A相无功功率
  if(temp>8388608) tx_buf[4] = temp - 16777216;
  else tx_buf[4] = temp;
  HT7038[2] = tx_buf[4] - feature_flash[0][2];  //减去无功偏置

  temp = att7053_Read(0x1c); //A相线频率
  //if(temp>8388608) tx_buf[5] = temp - 16777216;
  //else tx_buf[5] = temp;
  tx_buf[5] = temp;
  HT7038[3] = tx_buf[5];
  
  temp = att7053_Read(0x18);  //A相电流与电压相角
  if(temp>1048576) tx_buf[6] = temp - 16777216;
  else tx_buf[6] = temp;
  HT7038[4] = tx_buf[6] - feature_flash[0][4];//减去相角偏置
  
  tx_buf[7] = 0xfe;


  Feature_Buffer[0][buff_count] = HT7038[1];
  Feature_Buffer[1][buff_count] = HT7038[2];
  Feature_Buffer[2][buff_count] = HT7038[0]; //电流有效值
  buff_count++;
  if(buff_count>=buff_SIZE)
      buff_count = 0;
  
  printf("\r\n1.%d\r\n", tx_buf[2]);
//  printf("\r\n2.%d\r\n", tx_buf[3]);
//  printf("\r\n3.%d\r\n", tx_buf[4]);
//  printf("\r\n4.%d\r\n", tx_buf[5]);
//  printf("\r\n5.%d\r\n", tx_buf[6]);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if(huart->Instance == huart2.Instance)
   {
      if(UART2_RxBuffer == 0x01)
      {
          uartx_printf(huart2, "t8.txt=\"开始清理全部特征参数\"");
          HAL_UART_Transmit(&huart2,end,3,0xffff);
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