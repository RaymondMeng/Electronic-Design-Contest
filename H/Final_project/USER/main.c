#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "att7038au.h" 
 
/************************************************
初步思路：先把特殊的排除，再逐一进行条件判断
比如最大功率等
判断优先级：最大功率（最大电流） 最小功率 纯阻器件 

************************************************/
unsigned int CurrentDataCnt = 0;
void CurrentDataReadFun(void);
unsigned int currentDataBuf[8] = {0};  //存储计量ADC采集的临时数据
//unsigned int currentDataVal[12] = {0};  //存储经过校准和转化的电流数据，单位mA
//unsigned char currentCaliData[12] = {0};
int tx_buf[10] = {0};

unsigned int dat[7][2] = {0};//功率和功率因数，后面实测填值，如果过大就放入flash
unsigned int max, min;


u8 mode = 0; //0:识别模式  1:学习模式 

#define CURRENT_DAT_TIMES 256
#define DATA_RATIO 94
#define TYPE_NUM 7
#define FEATURE_NUM 5
//A相电流有效值 A相有功功率 A相无功功率 A相功率因数 A相电流与电压相角
uint32_t feature_flash[TYPE_NUM][FEATURE_NUM] = {28750, 695, 3860, 1480000, 460000,};
uint32_t distance[TYPE_NUM] = {0};
uint32_t HT7038[FEATURE_NUM] = {0};
float temp_f32 = 0.0;
uint32_t temp_u32 = 0;

void Find_Max_Min(void);

int main(void)
{		 
  

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
  uart3_init(115200);
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
  att7053_GPIO_Init();
	att7053_Init();
 	while(1)
	{
      //CurrentDataReadFun();
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//			}
//			printf("\r\n\r\n");//插入换行
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\r\n战舰STM32开发板 串口实验\r\n");
//				printf("正点原子@ALIENTEK\r\n\r\n");
//			}
//			if(times%200==0)printf("请输入数据,以回车键结束\n");  
//			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
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
//	  //计算当前特征与flash特征距离
//      for(i = 0;i<TYPE_NUM;i++){
//        for(j = 0;j<FEATURE_NUM;j++){
//          distance[i] += (feature_flash[i][j] - HT7038[j])*(feature_flash[i][j] - HT7038[j]);
//        }
//  //		  arm_sqrt_f32(distance[i], &temp_f32);
//  //		  distance[i] = temp_f32;
//        //printf("%d ", distance[i]);
//      }
//      //printf("\r\n");
//      //寻找距离最小的类别
//      temp_f32 = distance[0];
//      for(i = 0;i<TYPE_NUM;i++){
//        if(distance[i] < temp_f32){
//          temp_f32 = distance[i];
//          temp_u32 = i;
//        }
//      }
//	  //发送最小的类别

//      printf("class: %d\r\n",temp_u32);

      //if(!mode)
      //{
         /*首先判断是否为最大功率或者最小功率*/
         
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

	//currentDataBuf[0] += att7053_Read(0x0f); //C相电压有效值
	//currentDataBuf[0] += att7053_Read(0x0e); //B相电压有效值
  tx_buf[0] = 0xff;
	tx_buf[1] = att7053_Read(0x0D); //A相电压
	//currentDataBuf[1] += att7053_Read(0x12); //C相电流有效值
	tx_buf[2] = att7053_Read(0x10); //A相电流有效值
	//currentDataBuf[5] += att7053_Read(0x10); //A相电流有效值
  temp = att7053_Read(0x01); //A相有功功率
  if(temp>8388608) tx_buf[3] = temp - 16777216;
  else tx_buf[3] = temp;
  
  temp = att7053_Read(0x05); //A相无功功率
  if(temp>8388608) tx_buf[4] = temp - 16777216;
  else tx_buf[4] = temp;
  
  //tx_buf[5] = att7053_Read(POWERP1); //B相视在功率
  temp = att7053_Read(0x14); //A相功率因数
  if(temp>8388608) tx_buf[5] = temp - 16777216;
  else tx_buf[5] = temp;
  
  temp = att7053_Read(0x18);  //A相电流与电压相角
  if(temp>1048576) tx_buf[6] = temp - 16777216;
  else tx_buf[6] = temp;
  
  tx_buf[7] = 0xfe;

  //currentDataBuf[7] = att7053_Read(0x0e); //B相电压有效值
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
