/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_0
#define OLED_RST_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_1
#define OLED_CS_GPIO_Port GPIOB
#define DIR1_Pin GPIO_PIN_12
#define DIR1_GPIO_Port GPIOB
#define DIR2_Pin GPIO_PIN_13
#define DIR2_GPIO_Port GPIOB
#define TXD_Pin GPIO_PIN_9
#define TXD_GPIO_Port GPIOA
#define RXD_Pin GPIO_PIN_10
#define RXD_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//�???螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低�?�滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�???螺仪自检及测量范围，典型值：0x18(不自�???�???2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加�?�计自检、测量范围及高�?�滤波频率，典型值：0x01(不自�???�???2G�???5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型�?�：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存�???(默认数�??0x68，只�???)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据�???+1为读�???


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
