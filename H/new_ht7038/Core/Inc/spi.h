/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */
#define 		W_MODECFG					 0x01
#define 		W_MODULECFG				 0x31
#define			R_IARMS						 0x10
#define			R_UARMS						 0x0D
#define		  RES_VALUE				   5.1
#define     ACTIVE_Pa          0x01  //有功功率
#define     ACTIVE_Pb          0x02
#define     ACTIVE_Pc          0x03
#define     RECTIVE_Pa         0x05  //无功功率
#define     RECTIVE_Pb         0x06
#define     RECTIVE_Pc         0x07
#define     APPARENT_Pa        0x09  //视在功率
#define     APPARENT_Pb        0x0a  
#define     APPARENT_Pc        0x0b  
/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */

void ReadSpi(uint8_t Com, uint8_t* data);
void WriteSpi(uint8_t Com, uint8_t* data);
void HT7038ResetSet(void);
void SetHT7038Function(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
