#ifndef __ATT7053AU_H
#define __ATT7053AU_H

#include "stm32f10x.h"

#define att7053_CS_Low()  GPIO_ResetBits(GPIOE, GPIO_Pin_10)
#define att7053_CS_High()  GPIO_SetBits(GPIOE, GPIO_Pin_10)
#define att7053_SCK_Low()  GPIO_ResetBits(GPIOE, GPIO_Pin_12)
#define att7053_SCK_High()  GPIO_SetBits(GPIOE, GPIO_Pin_12)
#define att7053_MOSI_Low()  GPIO_ResetBits(GPIOE, GPIO_Pin_13)
#define att7053_MOSI_High()  GPIO_SetBits(GPIOE, GPIO_Pin_13)
#define att7053_MISO_STA() GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)



//#define Att7053CH2_CS_Low()  GPIO_ResetBits(GPIOE, GPIO_Pin_1)
//#define Att7053CH2_CS_High()  GPIO_SetBits(GPIOE, GPIO_Pin_1)
//#define Att7053CH2_SCK_Low()  GPIO_ResetBits(GPIOE, GPIO_Pin_0)
//#define Att7053CH2_SCK_High()  GPIO_SetBits(GPIOE, GPIO_Pin_0)
//#define Att7053CH2_MOSI_Low()  GPIO_ResetBits(GPIOB, GPIO_Pin_9)
//#define Att7053CH2_MOSI_High()  GPIO_SetBits(GPIOB, GPIO_Pin_9)
//#define Att7053CH2_MISO_STA() GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)


#define SPL_I1		0x00
#define SPL_I2		0x01  //A相有功功率
#define SPL_U		0x02    //B相有功功率
#define SPL_P		0x03    //C相有功功率
#define SPL_Q		0x04    
#define SPL_S		0x05    //A相无功功率
#define RMS_I1		0x06  //B相无功功率
#define RMS_I2		0x07  //C相无功功率
#define RMS_U		0x08
#define FREQ_U		0x09
#define POWERP1		0x0A   //B相视在功率
#define POWERQ1		0x0B   
#define POWER_S		0x0C
#define ENERGY_P	0x0D
#define ENERGY_Q	0x0E
#define ENERGY_S	0x0F
#define POWERP2		0x10
#define POWERQ2		0x11  //B相电流有效值
#define I1ANGLE 	0x12
#define I2ANGLE		0x13
#define TEMPDATA	0x14
#define GONGLYINS 0x15  //B相功率因数
#define BACKUPDATA	0x16
#define COMCHECKSUM	0x17
#define SUMCHECKSUM	0x18
#define EMUSR		0x19  //B相电流与电压相角
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

unsigned int Att7053CH2_Read(unsigned char addr);
void Att7053CH2_Write(unsigned char addr,unsigned int dat);
void Att7053CH2_GPIO_Init(void);
void Att7053CH2_Init(void);

#endif /* __ATT7053AU_H */
