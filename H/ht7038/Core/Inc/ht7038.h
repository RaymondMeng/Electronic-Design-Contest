#ifndef __HT7038_H
#define __HT7038_H

#include "sys.h"
//#include "delay.h"
#include "spi.h"

#define 	PACK_SIZE				3					//字长为3
#define		RES_VALUE				5.1					//电阻值5.1欧姆

#define 	HT7038_CS_PIN			PAout(4)			//片选引脚
#define 	HT7038_RST_PIN			PBout(9)			//复位引脚

#define		SPI_CS_PORT				GPIOA				//CS端口
#define		SPI_RST_PORT			GPIOB				//RST端口

#define 	CS_L()					HT7038_CS_PIN = 0	
#define 	CS_H()					HT7038_CS_PIN = 1	

#define 	RST_L()					HT7038_RST_PIN = 0
#define 	RST_H()					HT7038_RST_PIN = 1


/****************************** HT7038寄存器地址 ******************************/
//-------------->计量参数寄存器
#define			R_DEVICE_ID					0x00	//7038 Device ID(缺省值为	--	0x7126A0)

#define			R_UARMS						0x0D	//A相电压有效值(缺省值为	--	0x00)
#define			R_UBRMS						0x0E	//B相电压有效值
#define			R_UCRMS						0x0F	//C相电压有效值

#define			R_IARMS						0x10	//A相电流有效值(缺省值为	--	0x00)
#define			R_IBRMS						0x11	//B相电流有效值
#define			R_ICRMS						0x12	//C相电流有效值

#define			R_ITRMS						0x13	//3相电流矢量和的有效值(缺省值为	--	0x00)

//ADC采样数据寄存器
#define 		R_SAMPLE_IA					0x2f	//A相电流通道ADC采样数据(缺省值为	--	0x0)	
#define 		R_SAMPLE_IB					0x30
#define 		R_SAMPLE_IC					0x31
#define 		R_SAMPLE_UA					0x32	//A相电压通道ADC采样数据(缺省值为	--	0x0)
#define 		R_SAMPLE_UB					0x33
#define 		R_SAMPLE_UC					0x34

//-------------->校表参数寄存器
#define 		W_MODECFG					0x01	//模式相关控制(缺省值为			--	0x89aa)
#define 		W_MODULECFG					0x31	//电路模块配置寄存器(缺省值为	--	0x0x3527)
 
#define 		W_IGAINA					0x1a	//A相电流增益
#define 		W_IGAINB					0x1b	//B相电流增益
#define 		W_IGAINC					0x1c	//C相电流增益

#define 		W_IARMSOFFSE				0x27	//A相电流有效值offset校正
#define 		W_IBRMSOFFSE				0x28	//B相电流有效值offset校正
#define 		W_ICRMSOFFSE				0x29	//C相电流有效值offset校正
/************************************************************************************************/


extern uint8_t	APhaseReceBuf[PACK_SIZE];
extern uint8_t	BPhaseReceBuf[PACK_SIZE];

extern uint8_t 	EnableCorrectRegBuf[PACK_SIZE];
extern uint8_t 	EnableMeterRegBuf[PACK_SIZE];		
extern uint8_t 	ModeConfigBuf[PACK_SIZE];
extern uint8_t 	AnalogModuleConfigBuf[PACK_SIZE];


void HT7038_Init(void);
//void HT7038Reset(void);
//void HT7038ResetSet(uint8_t state);
////////////////////////////////////////////////////////////////////
void HT7038WriteSpecialCommand( uint8_t command, uint8_t *buffer);
void HT7038MeterORCorrectRegReadData( uint8_t command, uint8_t *buffer);
void HT7038CorrectRegWriteData( uint8_t command, uint8_t *buffer);
void SetHT7038Function(void);

#endif






