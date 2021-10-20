/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "control.h"
#include <math.h>
#include "stdio.h"
#include "retarget.h"
#include "inv_mpu.h"
#include "mpu6050.h"
#include "oled.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

float pitch,roll,yaw; 		//欧拉�?????????????????????
short aacx,aacy,aacz;
short gyrox,gyroy,gyroz;
short temp;	//温度
float velocity = 0;
uint8_t cnt=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void usart1_send_char(uint8_t c);
void usart1_niming_report(uint8_t fun,uint8_t*data,uint8_t len);
void mpu6050_send_data(float pitch,float roll,float yaw);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

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
	  uint8_t oled_buffer[16];


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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  RetargetInit(&huart1);


  OLED_ShowString(0, 0, "MPU start init", 16);
  OLED_Refresh();

  while(mpu_dmp_init())//
  	{
  	    printf("MPU6050 Error!!!\r\n");
        HAL_Delay(500);
  	}
  printf("MPU6050 OK\r\n");
  OLED_Clear();
  PID_Init();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{
				temp=MPU_Get_Temperature();
				MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);


					/*
					if((t%10)==0)
					{
						//tempÖµ
						if(temp<0)
						{
							temp=-temp;		//×ªÎªÕýÊý
							printf(" Temp:  -%d.%dC\r\n",temp/100,temp%10);
						}else
							printf(" Temp:  %d.%dC\r\n",temp/100,temp%10);

						//pitchÖµ
						temp=pitch*10;
						if(temp<0)
						{
							temp=-temp;		//×ªÎªÕýÊý
							printf(" Pitch: -%d.%dC\r\n",temp/10,temp%10);
						}else
							printf(" Pitch:  %d.%dC\r\n",temp/10,temp%10);

						//rollÖµ
						temp=roll*10;
						if(temp<0)
						{
							temp=-temp;		//×ªÎªÕýÊý
							printf(" Roll:  -%d.%dC\r\n",temp/10,temp%10);
						}else
							printf(" Roll:  %d.%dC\r\n",temp/10,temp%10);

						//yawÖµ
						temp=yaw*10;
						if(temp<0)
						{
							temp=-temp;		//×ªÎªÕýÊý
							printf(" Yaw:  -%d.%dC\r\n",temp/10,temp%10);
						}else
							printf(" Yaw:   %d.%dC\r\n",temp/10,temp%10);

						printf("\r\n");
						t=0;

					}  */

					mpu6050_send_data(pitch,roll,yaw);
					printf("  %f\r\n",velocity);

					sprintf(oled_buffer,"%f",pitch);
					OLED_ShowString(0, 0, oled_buffer, sizeof(oled_buffer));
					sprintf(oled_buffer,"%f",velocity);
					OLED_ShowString(0, 16, oled_buffer, sizeof(oled_buffer));
					OLED_Refresh();
					HAL_Delay(100);

	}
	//t++;

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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


  if(htim==(&htim1))
  {

	/***************test led****************/
	  if(cnt/200%2)
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);


	/*****************process*****************/

	setFrenquency(accel(pitch,&pid_pitch));
	if(cnt%128==0)
	{
		OLED_ClearWave();
		OLED_DrawLine(0, 48, 127, 48);
	}
	OLED_DrawPoint(cnt%128, 48+velocity/100*16);
	cnt++;
  }
}


void usart1_send_char(uint8_t c)
{
    while((USART1->SR&0X40)==0);//等待上一次发送完�?????????????????????
    USART1->DR=c;
}

//fun:功能�?????????????????????. 0XA0~0XAF
//data:数据缓存�?????????????????????,�?????????????????????�?????????????????????28字节!!
//len:data区有效数据个�?????????????????????
void usart1_niming_report(uint8_t fun,uint8_t*data,uint8_t len)
{
    uint8_t send_buf[32];
    uint8_t i;
    if(len>28)return;    //�?????????????????????�?????????????????????28字节数据
    send_buf[len+3]=0;  //校验数置�?????????????????????
    send_buf[0]=0X88;   //帧头
    send_buf[1]=fun;    //功能�?????????????????????
    send_buf[2]=len;    //数据长度
    for(i=0;i<len;i++)send_buf[3+i]=data[i];         //复制数据
    for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];    //计算校验�?????????????????????
    for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);   //发�?�数据到串口1
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
