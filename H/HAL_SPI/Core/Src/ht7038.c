/*
 * ht7038.c
 *
 *  Created on: Nov 6, 2021
 *      Author: zhang
 */
#include "ht7038.h"

#define DELAY 3




void delay(unsigned int cnt)
{
	unsigned int i,j;
	for(i=0;i<cnt;i++)
	{
		for(j=0;j<100;j++)
		{
			;
		}
	}
}


void att7053_Init(void)
{
	delay(10000);
	att7053_Write(0xC9,0x5A); //
	delay(100);
	att7053_Write(0x01,0x89fe); //
	delay(100);
	att7053_Write(0x31,0x3521);

	delay(10000);
	att7053_Write(0xc9,0x5b); //
	att7053_Write(0xc6,0x5b); //
	delay(10000);


}

void att7053_GPIO_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void att7053_Write(unsigned char addr,unsigned int dat)
{
	unsigned char i;

	att7053_CS_Low();
	att7053_SCK_Low();

	addr |= 0x80;
	delay(10);
	for(i=0;i<8;i++)
	{
		att7053_SCK_High();
		if(addr&0x80)
		{
			att7053_MOSI_High();
		}
		else
		{
			att7053_MOSI_Low();
		}

		addr = addr << 1;
		delay(DELAY);
		att7053_SCK_Low();
		delay(DELAY);
	}
	for(i=0;i<24;i++)
	{
		att7053_SCK_High();
		if(dat&0x800000)
		{
			att7053_MOSI_High();
		}
		else
		{
			att7053_MOSI_Low();
		}

		dat = dat << 1;
		delay(DELAY);
		att7053_SCK_Low();
		delay(DELAY);
	}

	att7053_CS_High();
	att7053_SCK_Low();
	att7053_MOSI_Low();
}

unsigned int att7053_Read(unsigned char addr)
{
	unsigned char i;
	unsigned int dat=0;

	att7053_CS_Low();
	att7053_SCK_Low();
	delay(10);

	for(i=0;i<8;i++)
	{
		att7053_SCK_High();
		if(addr&0x80)
		{
			att7053_MOSI_High();
		}
		else
		{
			att7053_MOSI_Low();
		}

		addr = addr << 1;
		delay(DELAY);
		att7053_SCK_Low();
		delay(DELAY);
	}

	delay(10);

	for(i=0;i<24;i++)
	{
		att7053_SCK_High();
		delay(DELAY);
		dat =dat << 1;
		if(att7053_MISO_STA())
		{
			dat |= 1;
		}

		att7053_SCK_Low();
		delay(DELAY);
	}

	att7053_CS_High();
	att7053_SCK_Low();
	att7053_MOSI_Low();

	return dat;
}


