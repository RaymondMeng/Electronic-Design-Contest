/**
  ******************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */
#define OLED_ADDRESS 0x78
/* USER CODE END Private defines */

void MX_I2C1_Init(void);


/* USER CODE BEGIN Prototypes */
void MX_I2C1_Init(void);
void WriteCmd(unsigned char);
void WriteData(unsigned char);

void OLED_Init(void);//OLED初始化
void OLED_SetPos(unsigned char, unsigned char);//放置光标点
void OLED_Fill(unsigned char);//充满
void OLED_CLS(void);//清屏
void OLED_ON(void);//开启
void OLED_OFF(void);//关闭

void OLED_ShowStr(unsigned char, unsigned char, unsigned char ch[], unsigned char);//写入字符
void OLED_ShowCN(unsigned char, unsigned char, unsigned char);//写入汉字
void OLED_DrawBMP(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char*);//画点位图
void Delay_us(unsigned char cnt);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
