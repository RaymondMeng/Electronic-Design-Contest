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
  * 第一个6050测杆与垂直线之间的角，第二个6050测激光与垂直线之间的角
  * 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "servo.h"
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
float pitch , roll, yaw;
short aacx, aacy, aacz;
short gyrox, gyroy, gyroz;
short temp;
float velocity = 0;
uint8_t cnt = 0;
float pitch2, pitch3; 

uint8_t UART1_RxBuffer, UART2_RxBuffer;
uint8_t buffer1[4] = "\0";
uint8_t mode; //几个测试点
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void usart1_send_char(uint8_t c);
void usart1_niming_report(uint8_t fun,uint8_t*data,uint8_t len);
void mpu6050_send_data(float pitch,float roll,float yaw);

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
  uint8_t txbuf = 1;
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //uartx_printf(huart1, "MPU6050 Error!!!\r\n");
/*********************************************  初始化  *****************************************/
//  while(mpu_dmp_init())//
//  	{
//  	    uartx_printf(huart1, "MPU6050 Error!!!\r\n");
//        HAL_Delay(500);
//  	}
//    uartx_printf(huart1, "MPU6050 OK\r\n");
    
  HAL_TIM_Base_Start_IT(&htim1); //开启定时器中断
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //开启PWM
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //开启PWM
//	HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART1_RxBuffer, 1); //开启串口中断
//  DBG("hello");

  Servo_Init(); //云台初始化，两自由度均为90°
  PID_Init(); //PID初始化
  //HAL_UART_Transmit(&huart1, &txbuf, 1, 100);
  //uartx_printf(huart1, "MPU6050 Error!!!\r\n");
  /* USER CODE END 2 */
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//    {
//				temp=MPU_Get_Temperature();
//				MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
//				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
//        mpu6050_send_data(pitch,roll,yaw);
//        uartx_printf(huart1, "roll: %f  pitch: %f  temp: %f\r\n", roll, pitch, temp);
//    }
    if(mode == 0x01) //模式1：垂直
      Tim_SetPWM(2, 1, positionx_pid.output); //定时器2通道1
    else if(mode == 0x02) //模式2：定点
      Tim_SetPWM(2, 2, positiony_pid.output);
    else if(mode == 0x03){ //模式3:画圆
//      Tim_SetPWM(2, 1, positionx_pid.output);
//      Tim_SetPWM(2, 2, positiony_pid.output);
    }
      
    //uartx_printf(huart1, "hello_world!\r\n");
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
void usart1_send_char(uint8_t c)
{
    while((USART1->SR&0X40)==0);//等待上一次发送完??????????????????????
    USART1->DR=c;
}

void usart1_niming_report(uint8_t fun,uint8_t*data,uint8_t len)
{
    uint8_t send_buf[32];
    uint8_t i;
    if(len>28)return;    //????????????????????????????????????????????28字节数据
    send_buf[len+3]=0;  //校验数置??????????????????????
    send_buf[0]=0X88;   //帧头
    send_buf[1]=fun;    //功能??????????????????????
    send_buf[2]=len;    //数据长度
    for(i=0;i<len;i++)send_buf[3+i]=data[i];         //复制数据
    for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];    //计算校验??????????????????????
    for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);   //发???数据到串口1
}

void mpu6050_send_data(float pitch,float roll,float yaw)
{
    uint8_t tbuf[16];
    unsigned char *p;
    p=(unsigned char *)&pitch;
    tbuf[0]=(unsigned char)(*(p+3));
    tbuf[1]=(unsigned char)(*(p+2));
    tbuf[2]=(unsigned char)(*(p+1));
    tbuf[3]=(unsigned char)(*(p+0));

    p=(unsigned char *)&roll;
    tbuf[4]=(unsigned char)(*(p+3));
    tbuf[5]=(unsigned char)(*(p+2));
    tbuf[6]=(unsigned char)(*(p+1));
    tbuf[7]=(unsigned char)(*(p+0));

    p=(unsigned char *)&yaw;
    tbuf[8]=(unsigned char)(*(p+3));
    tbuf[9]=(unsigned char)(*(p+2));
    tbuf[10]=(unsigned char)(*(p+1));
    tbuf[11]=(unsigned char)(*(p+0));

    usart1_niming_report(0XA2,tbuf,12);//自定义帧,0XA2
}

/*20ms为周期开始的中断*/
/**
  * @brief  定时器中断回调函数
  * @param  xxxx
  * @retval xxxx
  * @claim  我们只需要输入误差值，即返回要输入的pulses,如果采样频率过快可以后期增加
  * @claim  俯仰角前正后负，翻滚角左负右正
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

  //DBG("have received successfully!\r\n");
  //DBG("have received successfully!\r\n");
//  uint32_t output_temp;
  mode = buffer1[1];
  //uartx_printf(huart1, "MPU6050 Error!!!\r\n");
  if (htim->Instance == htim1.Instance){
    if(mode == 0x01)
    { //模式1
      //调节pid1为了使水平
      positionx_pid.measured_value =  pitch2_pulses; //传入测出的俯仰角
      positionx_pid.error          =  positionx_pid.measured_value - positionx_pid.set_value;
      positionx_pid.integral       =  positionx_pid.integral + positionx_pid.error * 0.02; //dt为采样频率，为0.02s
      positionx_pid.derival        =  (positionx_pid.error - positionx_pid.passive_error) / 0.02; //dt为采样频率（用定时器中断）  //判断一下是加还是减
      positionx_pid.output         =  current_pulses_x - (positionx_pid.kp * positionx_pid.error + positionx_pid.ki * positionx_pid.integral + positionx_pid.kd * positionx_pid.derival);
    //    positionx_pid.output         =  output_temp > 1500 ? 1500 : output_temp;
//    positionx_pid.output         =  output_temp <  500 ?  500 : output_temp;
      positionx_pid.passive_error  =  positionx_pid.error;
    }
    //DBG("have received successfully!\r\n");
    //模式2
    else if(mode == 0x02){  
      positiony_pid.measured_value =  pitch2_pulses;   //还是来自第二个舵机上的角度传感器的测量值，用于调节定点,需要pitch2进行转换
      positiony_pid.error          =  positiony_pid.measured_value - positiony_pid.set_value;
      positiony_pid.integral       =  positiony_pid.integral + positiony_pid.error * 0.02; //dt为采样频率
      positiony_pid.derival        =  (positiony_pid.error - positiony_pid.passive_error) / 0.02; //dt为采样频率（用定时器中断）   //判断一下是加还是减
      positiony_pid.output         =  current_pulses_y + (positiony_pid.kp * positiony_pid.error + positiony_pid.ki * positiony_pid.integral + positiony_pid.kd * positiony_pid.derival) ;
    //    positionx_pid.output         =  output_temp > 1500 ? 1500 : output_temp;
//    positionx_pid.output         =  output_temp <  500 ?  500 : output_temp;
      positiony_pid.passive_error  =  positiony_pid.error;
    }
    else{}//模式3  待写
  }
}

/**
  * @brief  串口2中断
  * @retval NONE
  * @claim  串口中断接收串口屏控制指令
  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  static uint8_t count = 0;
//  uartx_printf(huart1, "hello_world!\r\n");
//  if(huart->Instance == huart2.Instance)
//  {
//    if((count == 0) && (UART1_RxBuffer == 0xff)) //帧头
//    {
//      buffer1[0] = UART1_RxBuffer;
//      count++;      
//    }
//    else if(count == 1) //数据
//    {
//      buffer1[1] = UART1_RxBuffer;
//      count++;
//    }
//    else if((count == 2) && (UART1_RxBuffer == 0xfe)) //帧尾
//    {
//      buffer1[2] = UART1_RxBuffer;
//      count = 0;
//    }
//    else{
//      count = 0;
//      
//    }
//    HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART1_RxBuffer, 1); //重新开启中断
//    
//  }
//  
//}
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
