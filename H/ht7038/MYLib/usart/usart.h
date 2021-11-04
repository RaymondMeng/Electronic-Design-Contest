#ifndef __USART_H
#define __USART_H

#include "sys.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>


#define 	RX_BUFF_LEN 			128
#define 	TX_BUFF_LEN 			128
#define		BAUDRATE 				115200

typedef enum 
{
	IsUart,
	IsEthernet,
	IsOther
}DataSourceStruct;

typedef enum 
{
	IsNone,
	IsRecFinish
}RecStateStruct;

typedef enum 
{
	IsRecOkType,
	IsNoneType
}HandleTypeStruct;

extern HandleTypeStruct UartHandleType;

void 	Usart_X_Init(void);
void 	EnableDmaRec(u8 com);
u16 	GetUartData(u8 *buffer);
void	UsartDmaXSendData(u8 com,u8 *buf,u16 len);
void 	Usart_Init(u8 com,u32 bound,u16 Parity,u16 stopbit);	//串口函数初始化
void 	DMA_TX_Init(u8 com,u8 *Sbuffaddr);						//串口DMA传输初始化
void 	DMA_RX_Init(u8 com,u8 *Rbuffaddr);						//串口DMA接收初始化
void 	RTS_gpio_init(void);									//RTS引脚初始化

#endif
