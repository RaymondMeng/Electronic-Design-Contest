#include "att7053au.h"


void Att7053CH2_Init(void)
{
	delay(10000);
	Att7053CH2_Write(0xC9,0x5A); //
	delay(100);
	Att7053CH2_Write(0x01,0x89fe); // 
	delay(100);
	Att7053CH2_Write(0x31,0x3521);
	
	delay(10000);
	Att7053CH2_Write(0xc9,0x5b); //
	Att7053CH2_Write(0xc6,0x5b); // 
	delay(10000);
}
/*
ATT7053_SCK --> PB10
ATT7053_MOSI -->PB11
ATT7053_MISO -->PB12
ATT7053_CS  -->PB13
*/
void Att7053CH2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//void Att7053CH2_Write(unsigned char addr,unsigned int dat)
//{
//	unsigned char i;
//	
//	Att7053CH2_CS_Low();
//	Att7053CH2_SCK_Low();
//	
//	addr |= 0x80;
//	delay(10);
//	for(i=0;i<8;i++)
//	{
//		Att7053CH2_SCK_High();
//		if(addr&0x80)
//		{
//			Att7053CH2_MOSI_High();
//		}
//		else
//		{
//			Att7053CH2_MOSI_Low();
//		}
//		
//		addr = addr << 1;
//		delay(3);
//		Att7053CH2_SCK_Low();
//		delay(3);
//	}
//	for(i=0;i<24;i++)
//	{
//		Att7053CH2_SCK_High();
//		if(dat&0x800000)
//		{
//			Att7053CH2_MOSI_High();
//		}
//		else
//		{
//			Att7053CH2_MOSI_Low();
//		}
//		
//		dat = dat << 1;
//		delay(3);
//		Att7053CH2_SCK_Low();
//		delay(3);
//	}
//	
//	Att7053CH2_CS_High();
//	Att7053CH2_SCK_Low();
//	Att7053CH2_MOSI_Low();
//}
	
unsigned int Att7053CH2_Read(unsigned char addr)
{
	unsigned char i;
	unsigned int dat=0;
	
	Att7053CH2_CS_Low();
	Att7053CH2_SCK_Low();
	delay(10);
	
	for(i=0;i<8;i++)
	{
		Att7053CH2_SCK_High();
		if(addr&0x80)
		{
			Att7053CH2_MOSI_High();
		}
		else
		{
			Att7053CH2_MOSI_Low();
		}
		
		addr = addr << 1;
		delay(3);
		Att7053CH2_SCK_Low();
		delay(3);
	}
	
	delay(10);
	
	for(i=0;i<24;i++)
	{
		Att7053CH2_SCK_High();
		dat =dat << 1;
		if(Att7053CH2_MISO_STA())
		{
			dat |= 1;
		}
		delay(3);
		Att7053CH2_SCK_Low();
		delay(3);
	}
	
	Att7053CH2_CS_High();
	Att7053CH2_SCK_Low();
	Att7053CH2_MOSI_Low();
	
	return dat;	
}



/******************************END OF FILE*****************************/
