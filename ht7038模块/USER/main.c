#include <string.h>
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "ht7038.h"

#define		TEST_BUF_SIZE	50		//测试缓存大小

u8 	ReceData[3]= {0};
u16	count = 0;						//计数器
u16	data  = 0;

//保存原始电流有效值
static u32	A_Original_Buf[TEST_BUF_SIZE]={0};	//A相原始电流有效值
static u32	B_Original_Buf[TEST_BUF_SIZE]={0};	//B相原始电流有效值
static u32	C_Original_Buf[TEST_BUF_SIZE]={0};	//C相原始电流有效值

//保存转换后的电流有效值
static u16 a_changed_buf[TEST_BUF_SIZE] = {0};	//A相转换后的电流值
static u16 b_changed_buf[TEST_BUF_SIZE] = {0};	//B相转换后的电流值
static u16 c_changed_buf[TEST_BUF_SIZE] = {0};	//C相转换后的电流值


//主程序入口
int main(void)
{		
	u16		i = 0;
	float	temp = 0.0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();									//嘀嗒定时器初始化
	LEDx_Init();									//指示灯初始化
	SPIx_Init();									//SPI接口初始化
	Usart_X_Init();									//串口初始化
	TIM3_Int_Init(10-1,7200-1);						//定时器3初始化(定时时间	--	1ms)
	HT7038_Init();									//ht7038模块初始化
	SetHT7038Function();							//设置模块功能
	
	printf("ht7038模块测试！\r\n");
	
	while(1)
	{
		//读取数据的次数
		if( count < 20 )
		{
			//读取A相电流有效值
			HT7038MeterORCorrectRegReadData(R_IARMS,APhaseReceBuf);
			//读取B相电有效值//R_IBRMS
			HT7038MeterORCorrectRegReadData(R_UARMS,BPhaseReceBuf);
			
			A_Original_Buf[count] = (APhaseReceBuf[0]<<16) | (APhaseReceBuf[1]<<8) | (APhaseReceBuf[2]);
			//a_changed_buf[count] = (u16) (A_Rece_Buf[count]/8192/RES_VALUE*10);
			temp = (A_Original_Buf[count]/8192/RES_VALUE*10);
			a_changed_buf[count] = (u16)temp;

			temp = 0.0;

			B_Original_Buf[count] = (BPhaseReceBuf[0]<<16) | (BPhaseReceBuf[1]<<8) | (BPhaseReceBuf[2]);
			//b_changed_buf[count] = (u16) (B_Rece_Buf[count]/8192/RES_VALUE*10);
			temp = (B_Original_Buf[count]/8192/RES_VALUE*10);
			b_changed_buf[count] = (u16)temp;
			count++;			
		}
		else
		{
			printf("A相原始的电流有效值为：\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %2x  ",A_Original_Buf[i]);
			}
			printf("\r\n");
			printf("A相转换后的电流有效值为：\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %d  ",a_changed_buf[i]);
			}
			
			printf("\r\n");
			
			printf("B相原始的电压有效值为：\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %2x  ",B_Original_Buf[i]);
			}
			printf("\r\n");
			printf("B相转换后的电压有效值为：\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %d  ",b_changed_buf[i]);
			}
			
			count = 0;
			printf("\r\n******************************************************\r\n");
			printf("\r\n");
		}
		
		delay_ms(20);	//20ms读取一次
		
//		if(Timer3_Conuter>500)
//		{
//			Timer3_Conuter = 0;
//			HT7038MeterORCorrectRegReadData(W_MODECFG,ReceData);
//			data = (ReceData[2]<<16) |(ReceData[1]<<8) | (ReceData[2]);
//			printf("校表寄存器的值为	--	%02x\r\n",data);
//			LED_WORK = ~LED_WORK;
//		}
	}

	return 0;
}










