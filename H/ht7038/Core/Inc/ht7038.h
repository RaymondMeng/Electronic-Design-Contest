#ifndef __HT7038_H
#define __HT7038_H

#include "sys.h"
//#include "delay.h"
#include "spi.h"

#define 	PACK_SIZE				3					//�ֳ�Ϊ3
#define		RES_VALUE				5.1					//����ֵ5.1ŷķ

#define 	HT7038_CS_PIN			PAout(4)			//Ƭѡ����
#define 	HT7038_RST_PIN			PBout(9)			//��λ����

#define		SPI_CS_PORT				GPIOA				//CS�˿�
#define		SPI_RST_PORT			GPIOB				//RST�˿�

#define 	CS_L()					HT7038_CS_PIN = 0	
#define 	CS_H()					HT7038_CS_PIN = 1	

#define 	RST_L()					HT7038_RST_PIN = 0
#define 	RST_H()					HT7038_RST_PIN = 1


/****************************** HT7038�Ĵ�����ַ ******************************/
//-------------->���������Ĵ���
#define			R_DEVICE_ID					0x00	//7038 Device ID(ȱʡֵΪ	--	0x7126A0)

#define			R_UARMS						0x0D	//A���ѹ��Чֵ(ȱʡֵΪ	--	0x00)
#define			R_UBRMS						0x0E	//B���ѹ��Чֵ
#define			R_UCRMS						0x0F	//C���ѹ��Чֵ

#define			R_IARMS						0x10	//A�������Чֵ(ȱʡֵΪ	--	0x00)
#define			R_IBRMS						0x11	//B�������Чֵ
#define			R_ICRMS						0x12	//C�������Чֵ

#define			R_ITRMS						0x13	//3�����ʸ���͵���Чֵ(ȱʡֵΪ	--	0x00)

//ADC�������ݼĴ���
#define 		R_SAMPLE_IA					0x2f	//A�����ͨ��ADC��������(ȱʡֵΪ	--	0x0)	
#define 		R_SAMPLE_IB					0x30
#define 		R_SAMPLE_IC					0x31
#define 		R_SAMPLE_UA					0x32	//A���ѹͨ��ADC��������(ȱʡֵΪ	--	0x0)
#define 		R_SAMPLE_UB					0x33
#define 		R_SAMPLE_UC					0x34

//-------------->У������Ĵ���
#define 		W_MODECFG					0x01	//ģʽ��ؿ���(ȱʡֵΪ			--	0x89aa)
#define 		W_MODULECFG					0x31	//��·ģ�����üĴ���(ȱʡֵΪ	--	0x0x3527)
 
#define 		W_IGAINA					0x1a	//A���������
#define 		W_IGAINB					0x1b	//B���������
#define 		W_IGAINC					0x1c	//C���������

#define 		W_IARMSOFFSE				0x27	//A�������ЧֵoffsetУ��
#define 		W_IBRMSOFFSE				0x28	//B�������ЧֵoffsetУ��
#define 		W_ICRMSOFFSE				0x29	//C�������ЧֵoffsetУ��
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






