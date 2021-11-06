/*
 * ht7038.c
 *
 *  Created on: Nov 6, 2021
 *      Author: zhang
 */

#ifndef INC_HT7038_C_
#define INC_HT7038_C_

#include "main.h"
#include "gpio.h"



#define att7053_CS_Low()  		HAL_GPIO_WritePin(SPI_NSS_GPIO_Port,SPI_NSS_Pin,0)
#define att7053_CS_High()  		HAL_GPIO_WritePin(SPI_NSS_GPIO_Port,SPI_NSS_Pin,1)
#define att7053_SCK_Low()  		HAL_GPIO_WritePin(SPI_SCK_GPIO_Port,SPI_SCK_Pin,0)
#define att7053_SCK_High()  	HAL_GPIO_WritePin(SPI_SCK_GPIO_Port,SPI_SCK_Pin,1)
#define att7053_MOSI_Low()  	HAL_GPIO_WritePin(SPI_MOSI_GPIO_Port,SPI_MOSI_Pin,0)
#define att7053_MOSI_High() 	HAL_GPIO_WritePin(SPI_MOSI_GPIO_Port,SPI_MOSI_Pin,1)
#define att7053_MISO_STA() 		HAL_GPIO_ReadPin(SPI_MISO_GPIO_Port, SPI_MISO_Pin)


#define SPL_I1		0x00
#define SPL_I2		0x01
#define SPL_U		0x02
#define SPL_P		0x03
#define SPL_Q		0x04
#define SPL_S		0x05
#define RMS_I1		0x06
#define RMS_I2		0x07
#define RMS_U		0x08
#define FREQ_U		0x09
#define POWERP1		0x0A
#define POWERQ1		0x0B
#define POWER_S		0x0C
#define ENERGY_P	0x0D
#define ENERGY_Q	0x0E
#define ENERGY_S	0x0F
#define POWERP2		0x10
#define POWERQ2		0x11
#define I1ANGLE 	0x12
#define I2ANGLE		0x13
#define TEMPDATA	0x14
#define BACKUPDATA	0x16
#define COMCHECKSUM	0x17
#define SUMCHECKSUM	0x18
#define EMUSR		0x19
#define SYSSTA		0x1A
#define CHIPID		0x1B
#define DEVICEID	0x1C

#define	EMUIE		0x30
#define	EMUIF		0x31
#define	WPREG		0x32
#define	SRSTREG		0x33
#define	EMUCFG		0x40
#define	FREQCFG		0x41
#define	MODULEEN	0x42
#define	ANAEN		0x43
#define	IOCFG		0x45
#define	GP1			0x50
#define	GQ1			0x51
#define	GS1			0x52
#define PHASE1      0x53
#define	GP2			0x54
#define	GQ2			0x55
#define	GS2			0x56
#define PHASE2      0x57
#define	QPHSCAL		0x58
#define	ADCCON		0x59
#define	ALLGAIN		0x5A
#define	I2GAIN		0x5B
#define	I1OFF		0x5C
#define	I2OFF		0x5D
#define	UOFFF		0x5E
#define	PQSTART		0x5F
#define	PMSTART		0x5F
#define	HFCONST		0x61
#define	CHK			0x62
#define	IPTAMP		0x63
#define	UCONST		0x64
#define	P1OFFSETH	0x65
#define	P2OFFSETH	0x66
#define	Q1OFFSETH	0x67
#define	Q2OFFSETH	0x68
#define	I1RMSOFFSET	0x69
#define	I2RMSOFFSET	0x6A
#define	URMSOFFSET	0x6B
#define	ZCROSSCURRENT	0x6C
#define	GPHS1		0x6D
#define	GPHS2		0x6E
#define	PFCNT		0x6F
#define	QFCNT		0x70
#define	SFCNT		0x71

unsigned int att7053_Read(unsigned char addr);
void att7053_Write(unsigned char addr,unsigned int dat);
void att7053_GPIO_Init(void);
void att7053_Init(void);
void delay(unsigned int cnt);





#endif /* INC_HT7038_C_ */
