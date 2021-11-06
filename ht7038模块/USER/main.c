#include <string.h>
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "ht7038.h"

#define		TEST_BUF_SIZE	50		//���Ի����С

u8 	ReceData[3]= {0};
u16	count = 0;						//������
u16	data  = 0;

//����ԭʼ������Чֵ
static u32	A_Original_Buf[TEST_BUF_SIZE]={0};	//A��ԭʼ������Чֵ
static u32	B_Original_Buf[TEST_BUF_SIZE]={0};	//B��ԭʼ������Чֵ
static u32	C_Original_Buf[TEST_BUF_SIZE]={0};	//C��ԭʼ������Чֵ

//����ת����ĵ�����Чֵ
static u16 a_changed_buf[TEST_BUF_SIZE] = {0};	//A��ת����ĵ���ֵ
static u16 b_changed_buf[TEST_BUF_SIZE] = {0};	//B��ת����ĵ���ֵ
static u16 c_changed_buf[TEST_BUF_SIZE] = {0};	//C��ת����ĵ���ֵ


//���������
int main(void)
{		
	u16		i = 0;
	float	temp = 0.0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();									//��શ�ʱ����ʼ��
	LEDx_Init();									//ָʾ�Ƴ�ʼ��
	SPIx_Init();									//SPI�ӿڳ�ʼ��
	Usart_X_Init();									//���ڳ�ʼ��
	TIM3_Int_Init(10-1,7200-1);						//��ʱ��3��ʼ��(��ʱʱ��	--	1ms)
	HT7038_Init();									//ht7038ģ���ʼ��
	SetHT7038Function();							//����ģ�鹦��
	
	printf("ht7038ģ����ԣ�\r\n");
	
	while(1)
	{
		//��ȡ���ݵĴ���
		if( count < 20 )
		{
			//��ȡA�������Чֵ
			HT7038MeterORCorrectRegReadData(R_IARMS,APhaseReceBuf);
			//��ȡB�����Чֵ//R_IBRMS
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
			printf("A��ԭʼ�ĵ�����ЧֵΪ��\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %2x  ",A_Original_Buf[i]);
			}
			printf("\r\n");
			printf("A��ת����ĵ�����ЧֵΪ��\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %d  ",a_changed_buf[i]);
			}
			
			printf("\r\n");
			
			printf("B��ԭʼ�ĵ�ѹ��ЧֵΪ��\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %2x  ",B_Original_Buf[i]);
			}
			printf("\r\n");
			printf("B��ת����ĵ�ѹ��ЧֵΪ��\r\n");
			for(i=0;i<count;i++)
			{
				printf(" %d  ",b_changed_buf[i]);
			}
			
			count = 0;
			printf("\r\n******************************************************\r\n");
			printf("\r\n");
		}
		
		delay_ms(20);	//20ms��ȡһ��
		
//		if(Timer3_Conuter>500)
//		{
//			Timer3_Conuter = 0;
//			HT7038MeterORCorrectRegReadData(W_MODECFG,ReceData);
//			data = (ReceData[2]<<16) |(ReceData[1]<<8) | (ReceData[2]);
//			printf("У��Ĵ�����ֵΪ	--	%02x\r\n",data);
//			LED_WORK = ~LED_WORK;
//		}
	}

	return 0;
}










