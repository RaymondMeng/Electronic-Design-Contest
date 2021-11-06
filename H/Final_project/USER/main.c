#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "att7038au.h" 
 
/************************************************
����˼·���Ȱ�������ų�������һ���������ж�
��������ʵ�
�ж����ȼ�������ʣ��������� ��С���� �������� 

************************************************/
unsigned int CurrentDataCnt = 0;
void CurrentDataReadFun(void);
unsigned int currentDataBuf[8] = {0};  //�洢����ADC�ɼ�����ʱ����
//unsigned int currentDataVal[12] = {0};  //�洢����У׼��ת���ĵ������ݣ���λmA
//unsigned char currentCaliData[12] = {0};
int tx_buf[10] = {0};

unsigned int dat[7][2] = {0};//���ʺ͹�������������ʵ����ֵ���������ͷ���flash
unsigned int max, min;


u8 mode = 0; //0:ʶ��ģʽ  1:ѧϰģʽ 

#define CURRENT_DAT_TIMES 256
#define DATA_RATIO 94
#define TYPE_NUM 7
#define FEATURE_NUM 5
//A�������Чֵ A���й����� A���޹����� A�๦������ A��������ѹ���
uint32_t feature_flash[TYPE_NUM][FEATURE_NUM] = {28750, 695, 3860, 1480000, 460000,};
uint32_t distance[TYPE_NUM] = {0};
uint32_t HT7038[FEATURE_NUM] = {0};
float temp_f32 = 0.0;
uint32_t temp_u32 = 0;

void Find_Max_Min(void);

int main(void)
{		 
  

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
  uart3_init(115200);
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
  att7053_GPIO_Init();
	att7053_Init();
 	while(1)
	{
      //CurrentDataReadFun();
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
//			printf("\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\r\nս��STM32������ ����ʵ��\r\n");
//				printf("����ԭ��@ALIENTEK\r\n\r\n");
//			}
//			if(times%200==0)printf("����������,�Իس�������\n");  
//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
    //if(CurrentDataCnt < CURRENT_DAT_TIMES)
		//{
			//CurrentDataCnt++;
      int i, j;
			CurrentDataReadFun();
//      for(i = 0;i<TYPE_NUM;i++)
//        for(j = 0;j<FEATURE_NUM;j++)
//          //printf("%x ",feature_flash[i][j]);
//        //printf("\r\n");
//      
//	  //���㵱ǰ������flash��������
//      for(i = 0;i<TYPE_NUM;i++){
//        for(j = 0;j<FEATURE_NUM;j++){
//          distance[i] += (feature_flash[i][j] - HT7038[j])*(feature_flash[i][j] - HT7038[j]);
//        }
//  //		  arm_sqrt_f32(distance[i], &temp_f32);
//  //		  distance[i] = temp_f32;
//        //printf("%d ", distance[i]);
//      }
//      //printf("\r\n");
//      //Ѱ�Ҿ�����С�����
//      temp_f32 = distance[0];
//      for(i = 0;i<TYPE_NUM;i++){
//        if(distance[i] < temp_f32){
//          temp_f32 = distance[i];
//          temp_u32 = i;
//        }
//      }
//	  //������С�����

//      printf("class: %d\r\n",temp_u32);

      //if(!mode)
      //{
         /*�����ж��Ƿ�Ϊ����ʻ�����С����*/
         
      //}
      //else
      //{
        
      //}
      //USART_SendData(USART2, temp);
//		}
//		else if(CurrentDataCnt == CURRENT_DAT_TIMES)
//		{
//			CurrentDataCnt = 0;
//      
//			for(i=0; i<12; i++)
//			{
//				currentDataVal[i] = currentDataBuf[i]/(DATA_RATIO*CURRENT_DAT_TIMES);
//				
//				if(currentDataVal[i] > currentCaliData[i])
//				{
//					currentDataVal[i] = currentDataVal[i] - currentCaliData[i];
//				}
//				else
//				{
//					currentDataVal[i] = currentCaliData[i] - currentDataVal[i];
//				}
//				
//				currentDataBuf[i] = 0;
//			}
//    }
//    else
//    {
//      CurrentDataCnt = 0;
//			for(j=0; j<12; j++)
//			{
//				currentDataBuf[j] = 0;
//			}
//    }
    
    
	}	 
}


void CurrentDataReadFun()
{
  int i = 0;
  int temp = 0;
//	currentDataBuf[6] = Att7053CH2_Read(0x0f);
//	currentDataBuf[7] = Att7053CH2_Read(0x0e);
//	currentDataBuf[8] = Att7053CH2_Read(0x0d);
//	currentDataBuf[9] = Att7053CH2_Read(0x12);
//	currentDataBuf[10] = Att7053CH2_Read(0x11);
//	currentDataBuf[11] = Att7053CH2_Read(0x10);

	//currentDataBuf[0] += att7053_Read(0x0f); //C���ѹ��Чֵ
	//currentDataBuf[0] += att7053_Read(0x0e); //B���ѹ��Чֵ
  tx_buf[0] = 0xff;
	tx_buf[1] = att7053_Read(0x0D); //A���ѹ
	//currentDataBuf[1] += att7053_Read(0x12); //C�������Чֵ
	tx_buf[2] = att7053_Read(0x10); //A�������Чֵ
	//currentDataBuf[5] += att7053_Read(0x10); //A�������Чֵ
  temp = att7053_Read(0x01); //A���й�����
  if(temp>8388608) tx_buf[3] = temp - 16777216;
  else tx_buf[3] = temp;
  
  temp = att7053_Read(0x05); //A���޹�����
  if(temp>8388608) tx_buf[4] = temp - 16777216;
  else tx_buf[4] = temp;
  
  //tx_buf[5] = att7053_Read(POWERP1); //B�����ڹ���
  temp = att7053_Read(0x14); //A�๦������
  if(temp>8388608) tx_buf[5] = temp - 16777216;
  else tx_buf[5] = temp;
  
  temp = att7053_Read(0x18);  //A��������ѹ���
  if(temp>1048576) tx_buf[6] = temp - 16777216;
  else tx_buf[6] = temp;
  
  tx_buf[7] = 0xfe;

  //currentDataBuf[7] = att7053_Read(0x0e); //B���ѹ��Чֵ
  printf("\r\n1.%d\r\n", tx_buf[4]);
  printf("\r\n2.%d\r\n", tx_buf[2]);
  printf("\r\n3.%d\r\n", tx_buf[3]);
  printf("\r\n4.%d\r\n", tx_buf[5]);
  printf("\r\n5.%d\r\n", tx_buf[6]);
  //printf("\r\n2.%d\r\n", tx_buf[2]);
  //printf("\r\n3.%d\r\n", tx_buf[3]);
  //printf("\r\n4.%d\r\n", tx_buf[5]);
  //USART_SendData(USART1, tx_buf[1]);
//  printf("\r\n");
  //USART_SendData(USART1, tx_buf[1]);
//  for(i = 1; i < 7; i++)
//  {
//     USART_SendData(USART1, tx_buf[i]>>24);
//     USART_SendData(USART1, (tx_buf[i]>>16)&0x0ff);
//     USART_SendData(USART1, (tx_buf[i]>>8)&0x0ff);
//     USART_SendData(USART1, tx_buf[i]&0x0ff);
//  }
//  printf("\r\n");
  //USART_SendData(USART1, tx_buf[7]);
  //printf("%d", currentDataBuf[4]);
  //USART_SendData(USART1,currentDataBuf[4]);
}
