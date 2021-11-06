/******************************************************************************
 * �ļ���  ��main.c
******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "usart1.h"
#include "att7053au.h"
#include "bp.h"

#define CALI_FLAG 200
#define CALI_DATA_ADDR 10
#define DEV_ADDR 0x01
#define CURRENT_DAT_TIMES 256
#define DAT_TIMES 256
#define DATA_RATIO 94
#define VALUE_CHANGE_MAX 20

#define I2C_SPEED 100000
#define I2C1_OWN_ADDRESS 0x0A
#define I2C_PAGE_SIZE 8
#define ON 1
#define OFF 0

void LED_GPIO_Init(void);
void LED_WorkStaCon(unsigned char ch,unsigned char sta);
void Relay_GPIO_Init(void);
void Relay_Work_Mod_Con(unsigned char ch,unsigned char mod);
void RelayStatusDatInit(void);
void WorkControlPin_Init(void);

void Uart5_Init(void);
void Usart3_Init(void);
void Usart2_Init(void);

void EEPROM_Init(void);
void I2C_EE_BufferWrite(unsigned char* pBuffer, unsigned char WriteAddr, unsigned int NumByteToWrite);
void I2C_EE_ByteWrite(unsigned char* pBuffer, unsigned char WriteAddr);
void I2C_EE_PageWrite(unsigned char* pBuffer, unsigned char WriteAddr, unsigned int NumByteToWrite);
void I2C_EE_BufferRead(unsigned char* pBuffer, unsigned char ReadAddr, unsigned int NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);

void Delay(__IO uint32_t nCount);
void TIM2_Start(void);
void TIM2_Stop(void);
void TIM2_Configuration(void);
void msDelay(unsigned int cnt);
void USART1_SendData(unsigned char dat);
void SlaveDev_Reset_Init(void);

void AnalogDat_Detect_Init(void);
unsigned int AnalogDat_Read(unsigned char ch);
void AnalogDatSetToBuf(void);

void CurrentDataReadFun(void);
void CurrentDataCalibrateFunction(void);
void CurrentDataASetToBuf(void);
void CurrentDataBSetToBuf(void);
void CurrentDataSendOverUart(void);

void SensorDataSendOverUart(unsigned char SenssorAddr);
void DataBufInit(void);

void PressureDataDebugOverUART(void);

unsigned int eeprom_Addr = 0;

unsigned char currentCaliData[12] = {0};
unsigned int currentDataBuf[12] = {0};  //�洢����ADC�ɼ�����ʱ����

unsigned int currentDataVal[12] = {0};  //�洢����У׼��ת���ĵ������ݣ���λmA
unsigned char uartCurrentData[15]= {0}; 

unsigned char analogDataMeasFlag = 0;
unsigned int xCnt = 0;
unsigned int dataReadTemp = 0;
unsigned int analogDataCnt = 0;
unsigned int analogPreData[6] = {0};
unsigned int analogDataBuf[6] = {0};    //�洢�ڲ�ADC�ɼ�����ʱ����
unsigned int convertAnalogData[6] = {0};   //�洢�������ݴ�����¶����ݻ���ѹ������
unsigned char uartAnalogData[15] = {0};


unsigned char uartCommFlag = 0;
unsigned int delayTimCnt = 0;
unsigned int timMinuteCnt = 0;
unsigned char timMinuteFlag = 0;
unsigned char UartDatBuf[200];
unsigned int CurrentDataCnt = 0;

unsigned char tempAcommFlag = 0;
unsigned char tempAcommCnt = 0;	
unsigned char tempADataFlag = 0;
unsigned char tempAcommDataBuf[12];
unsigned char uarttempDataBufA[15];
unsigned char tempADataTickFlag = 0;

//ÿ��10S���һ�α�־λ��״̬������λ��ʾ�������յ��¶����ݣ�����λ��ʾ�ӻ�����������
unsigned char tempTickDetectFlag = 0;
unsigned int tempTickDetectCnt = 0;

unsigned char tempBcommFlag = 0;
unsigned char tempBcommCnt = 0;
unsigned char tempBDataFlag = 0;	
unsigned char tempBcommDataBuf[12];
unsigned char uarttempDataBufB[15];
unsigned char tempBDataTickFlag = 0;

unsigned char uartWorkDat[6][6];
unsigned char uartWorkMode[6] = {0,0,0,0,0,0};
unsigned char uart1CommBuf[6];
unsigned char uart1CommFlag = 0;
unsigned char uart1CommCnt = 0;


unsigned int relayWorkTimeCnt[8];
unsigned char relayWorkTimeFlag[8];
unsigned char relayWorkSta[15];
unsigned char relayDataSendFlag = 0;
unsigned int relayDataSendCnt = 0;
unsigned char relayAllOffFlag = 0;

unsigned char testSta = 0;

uint8_t BP_in[2], BP_out[1]; 




int main(void)
{ 	
	unsigned char i = 0;
	WorkControlPin_Init();
	SlaveDev_Reset_Init();
	GPIO_ResetBits(GPIOD , GPIO_Pin_3);//--Reset MCU,Wait
	GPIO_ResetBits(GPIOE , GPIO_Pin_14);//--Reset MCU,Wait
	
	//TIM2_Configuration();
	//TIM2_Start();   //��ʼ����ʱ��������
	UART_Init(115200);
	//Usart3_Init();
	//Uart5_Init();
	Usart2_Init();
	
	att7053_GPIO_Init();
	att7053_Init();
	//Att7053CH2_GPIO_Init();
	//Att7053CH2_Init();
	
	while(1)
	{
    //BP��ʼ��ϣ�ȡ����ȷ����
    simInit();
//    BP_in[0] = ObjectTemperature;
//    BP_in[1] = Distance;
    
		//--�������ݶ�ȡ,�����ݴ���
		if(CurrentDataCnt < CURRENT_DAT_TIMES)
		{
			CurrentDataCnt++;
			CurrentDataReadFun();
		}
		else if(CurrentDataCnt == CURRENT_DAT_TIMES)
		{
			CurrentDataCnt = 0;
			for(i=0;i<12;i++)
			{
				currentDataVal[i] = currentDataBuf[i]/(DATA_RATIO*CURRENT_DAT_TIMES);
				
				if(currentDataVal[i] > currentCaliData[i])
				{
					currentDataVal[i] = currentDataVal[i] - currentCaliData[i];
				}
				else
				{
					currentDataVal[i] = currentCaliData[i] - currentDataVal[i];
				}
				
				currentDataBuf[i] = 0;
			}
			//CurrentDataASetToBuf();
			//SensorDataSendOverUart(4);
			//CurrentDataBSetToBuf();
			//SensorDataSendOverUart(5);
      //USART1_SendData();
      
      //printf("\r\nB���й������ǣ�%d\r\n", currentDataVal[2]);
      //printf("\r\nB���޹������ǣ�%d\r\n", currentDataVal[3]);
      //printf("\r\nB�����ڹ����ǣ�%d\r\n", currentDataVal[4]);
      //printf("\r\nB�๦�������ǣ�%d\r\n", currentDataVal[5]);
      //printf("\r\nB��������ѹ����ǣ�%d\r\n", currentDataVal[6]);
		}
		else //���
		{
			CurrentDataCnt = 0;
			for(i=0;i<12;i++)
			{
				currentDataBuf[i] = 0;
			}
		}
    USART_SendData(USART2, currentDataVal[1]);
    //printf("\r\nB�������Чֵ�ǣ�%d\r\n", currentDataVal[1]);
    //printf("\r\nA���ѹ��Чֵ�ǣ�%d\r\n", currentDataVal[0]);
    //printf("���");
		//�̵�����������
    
  }
		
	
}

//�忨����״̬��������PA8,PA11
//PA8���ż�⵽�͵�ƽ��忨��ʼ�������ߵ�ƽʱ�ȴ�MasterBoard��ʼ��
//PA11������Ϊ�忨�������������������
//PA11=0�忨ִ�и�λ������PA=1�忨��������
void WorkControlPin_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// ���豸MCU��λ��״̬֪ͨλ
//PD3=0 ---> MCU RESET;PD3=1 ---> MCU WORK
//PE14=0 ---> MCU RESET;PE14=1 ---> MCU WORK
//PD0=0 ---> WAIT;PD0=1 ---> WORK
//PE15=0 ---> WAIT;PE15=1 ---> WORK
void SlaveDev_Reset_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD , GPIO_Pin_0 | GPIO_Pin_3 );
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE , GPIO_Pin_14 | GPIO_Pin_15 );
}
void PressureDataDebugOverUART()
{
	unsigned char i;
	
	analogDataBuf[0] = analogDataBuf[0];
	for(i=0;i<6;i++)
	{
		
		printf("CH%d == %d\r\n",i+1,analogDataBuf[i]);
		analogDataBuf[i] = 0;
	}
}
void DataBufInit(void)
{
	unsigned char i;
	for(i=0;i<14;i++)
	{
		uarttempDataBufA[i] = 0;
		uarttempDataBufB[i] = 0;
	}
	
	uarttempDataBufA[0] = 0xfa; uarttempDataBufA[1] = 1; uarttempDataBufA[14] = 0xfb;
	uarttempDataBufB[0] = 0xfa; uarttempDataBufB[1] = 2; uarttempDataBufB[14] = 0xfb;
	uartAnalogData[0] = 0xfa; uartAnalogData[1] = 3; uartAnalogData[14] = 0xfb;
}
void SensorDataSendOverUart(unsigned char SenssorAddr)
{
	unsigned char i;
	unsigned char dat;
	
	LED_WorkStaCon(3,ON);
	
	dat = SenssorAddr;
	if(dat == 1) //temp CH1-6
	{
		for(i=0;i<15;i++)
		{
			USART1_SendData(uarttempDataBufA[i]);
		}
	}
	else if(dat == 2) //temp CH7-12
	{
		for(i=0;i<15;i++)
		{
			USART1_SendData(uarttempDataBufB[i]);
		}
	}
	else if(dat == 3) //pressure Ch1-6
	{
		for(i=0;i<15;i++)
		{
			USART1_SendData(uartAnalogData[i]);
		}
	}
	else if(dat == 4) //Current Ch1-6
	{
		for(i=0;i<15;i++)
		{
			USART1_SendData(uartCurrentData[i]);
		}
	}
	else if(dat == 5) //Current Ch7-12
	{
		for(i=0;i<15;i++)
		{
			USART1_SendData(uartCurrentData[i]);
		}
	}
	else if(dat == 6) //Relay Status CH1-6
	{
		for(i=0;i<15;i++)
		{
			USART1_SendData(relayWorkSta[i]);
		}
	}
	LED_WorkStaCon(3,OFF);
}

void AnalogDatSetToBuf(void)
{
	unsigned char i;
	uartAnalogData[0] = 0xfa;
	uartAnalogData[1] = 3;
	uartAnalogData[14] = 0xfb;
	for(i=0;i<6;i++)
	{
		uartAnalogData[2*i+2] = (unsigned char)(analogDataBuf[i] & 0xff);
		uartAnalogData[2*i+3] = (unsigned char)((analogDataBuf[i] >> 8) & 0xff);
		
		analogDataBuf[i] = 0;
	}
	for(i=2;i<14;i++)
	{
		if((uartAnalogData[i] == 0xfa)||(uartAnalogData[i] == 0xfb))
		{
			uartAnalogData[i] = 0xfc;
		}
	}
}

void AnalogDat_Detect_Init()
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1 , ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //����ADC��Ƶ����8 72M/8=9,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


unsigned int AnalogDat_Read(unsigned char ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}




void Usart3_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	USART_DeInit(USART3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  	NVIC_Init(&NVIC_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������5
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 
	
}

void Usart2_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	USART_DeInit(USART2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  	NVIC_Init(&NVIC_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 
	
}

void Uart5_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	USART_DeInit(UART5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  	NVIC_Init(&NVIC_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//�շ�ģʽ

	USART_Init(UART5, &USART_InitStructure); //��ʼ������5
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���1 
	
}




void CurrentDataReadFun()
{
//	currentDataBuf[6] = Att7053CH2_Read(0x0f);
//	currentDataBuf[7] = Att7053CH2_Read(0x0e);
//	currentDataBuf[8] = Att7053CH2_Read(0x0d);
//	currentDataBuf[9] = Att7053CH2_Read(0x12);
//	currentDataBuf[10] = Att7053CH2_Read(0x11);
//	currentDataBuf[11] = Att7053CH2_Read(0x10);

	//currentDataBuf[0] += att7053_Read(0x0f); //C���ѹ��Чֵ
	//currentDataBuf[0] += att7053_Read(0x0e); //B���ѹ��Чֵ
	currentDataBuf[0] += att7053_Read(0x0d); //A���ѹ��Чֵ
	//currentDataBuf[1] += att7053_Read(0x12); //C�������Чֵ
	currentDataBuf[1] += att7053_Read(0x11); //B�������Чֵ
	//currentDataBuf[5] += att7053_Read(0x10); //A�������Чֵ
  currentDataBuf[2] += att7053_Read(SPL_U); //B���й�����
  currentDataBuf[3] += att7053_Read(RMS_I1); //B���޹�����
  currentDataBuf[4] += att7053_Read(POWERP1); //B�����ڹ���
  currentDataBuf[5] += att7053_Read(GONGLYINS); //B�๦������
  currentDataBuf[6] += att7053_Read(EMUSR);  //B��������ѹ���
  
  //printf("%d", currentDataBuf[4]);
  //USART_SendData(USART1,currentDataBuf[4]);
}

void CurrentDataCalibrateFunction()
{
	unsigned int i;
	for(i=0;i<12;i++)
	{
		currentDataBuf[i] = 0;
	}
	for(i=0;i<CURRENT_DAT_TIMES;i++)
	{
		CurrentDataReadFun();
	}
	for(i=0;i<12;i++)
	{
		currentCaliData[i] = (unsigned char)(currentDataBuf[i]/(DATA_RATIO*CURRENT_DAT_TIMES) );
		
		currentDataBuf[i] = 0;
	}
	
	I2C_EE_BufferWrite(currentCaliData,CALI_DATA_ADDR+1,12);
	
}


void CurrentDataASetToBuf(void)
{
	unsigned char i;
	uartCurrentData[0] = 0xfa;
	uartCurrentData[1] = 4;
	uartCurrentData[14] = 0xfb;
	for(i=0;i<6;i++)
	{
		uartCurrentData[2*i+2] = (unsigned char)(currentDataVal[i] & 0xff);
		uartCurrentData[2*i+3] = (unsigned char)((currentDataVal[i] >> 8) & 0xff);
		
		currentDataVal[i] = 0;
	}
	for(i=2;i<14;i++)
	{
		if((uartCurrentData[i] == 0xfa)||(uartCurrentData[i] == 0xfb))
		{
			uartCurrentData[i] = 0xfc;
		}
	}
}

void CurrentDataBSetToBuf(void)
{
	unsigned char i;
	uartCurrentData[0] = 0xfa;
	uartCurrentData[1] = 5;
	uartCurrentData[14] = 0xfb;
	for(i=0;i<6;i++)
	{
		uartCurrentData[2*i+2] = (unsigned char)(currentDataVal[i+6] & 0xff);
		uartCurrentData[2*i+3] = (unsigned char)((currentDataVal[i+6] >> 8) & 0xff);
		
		currentDataVal[i] = 0;
	}
	for(i=2;i<14;i++)
	{
		if((uartCurrentData[i] == 0xfa)||(uartCurrentData[i] == 0xfb))
		{
			uartCurrentData[i] = 0xfc;
		}
	}
}

void CurrentDataSendOverUart()
{
	unsigned char i;
	for(i=0;i<12;i++)
	{
		printf("CH%d == %d,CaliDat == %d\r\n",i,currentDataVal[i],currentCaliData[i]);
	}
}
void I2C_Mode_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 =I2C1_OWN_ADDRESS; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* ʹ�� I2C1 */
  I2C_Cmd(I2C1, ENABLE);

  /* I2C1 ��ʼ�� */
  I2C_Init(I2C1, &I2C_InitStructure);

   
}
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* ʹ���� I2C1 �йص�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
    
  /* PB6-I2C1_SCL��PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ���õ�ַ����
	// EEPROM_A0: GPIOB_PIN5
	// EEPROM_A1: GPIOB_PIN4
	// EEPROM_A2: GPIOB_PIN3
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 			   															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����ַ����ΪA0~A2 = 0		  
	GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
}

void EEPROM_Init(void)
{

  I2C_GPIO_Config(); 
 
  I2C_Mode_Config();

  eeprom_Addr = 0xA0;

}

void I2C_EE_PageWrite(unsigned char* pBuffer, unsigned char WriteAddr, unsigned int NumByteToWrite)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); 
	
	I2C_GenerateSTART(I2C1, ENABLE);// Send START condition 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); //Test on EV5 and clear it
	I2C_Send7bitAddress(I2C1, eeprom_Addr, I2C_Direction_Transmitter);//Send EEPROM address for write 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  /* Test on EV6 and clear it */
	I2C_SendData(I2C1, WriteAddr);   /* Send the EEPROM's internal address to write to */ 
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  /* Test on EV8 and clear it */
	while(NumByteToWrite--)  /* While there is data to be written */ 
	{
		I2C_SendData(I2C1, *pBuffer); /* Send the current byte */
		pBuffer++; /* Point to the next byte to be written */
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/* Test on EV8 and clear it */
	}
	I2C_GenerateSTOP(I2C1, ENABLE); /* Send STOP condition */
}
void I2C_EE_ByteWrite(unsigned char* pBuffer, unsigned char WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, eeprom_Addr, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, WriteAddr);
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C1, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_EE_BufferWrite(unsigned char* pBuffer, unsigned char WriteAddr, unsigned int NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PAGE_SIZE;
  count = I2C_PAGE_SIZE - Addr;
  NumOfPage =  NumByteToWrite / I2C_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % I2C_PAGE_SIZE;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PAGE_SIZE); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PAGE_SIZE;
        pBuffer += I2C_PAGE_SIZE;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PAGE_SIZE;
      NumOfSingle = NumByteToWrite % I2C_PAGE_SIZE;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PAGE_SIZE);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PAGE_SIZE;
        pBuffer += I2C_PAGE_SIZE;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}

void I2C_EE_BufferRead(unsigned char* pBuffer, unsigned char ReadAddr, unsigned int NumByteToRead)
 {  
  //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, eeprom_Addr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, eeprom_Addr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, eeprom_Addr, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(I2C1, ENABLE); 
}


void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE , GPIO_Pin_2);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB , GPIO_Pin_12 | GPIO_Pin_13 );
}

void LED_WorkStaCon(unsigned char ch,unsigned char sta)
{
	if((ch == 1)&&(sta == ON)) GPIO_ResetBits(GPIOB , GPIO_Pin_12);
	else if((ch == 1)&&(sta == OFF)) GPIO_SetBits(GPIOB , GPIO_Pin_12);
	else if((ch == 2)&&(sta == ON)) GPIO_ResetBits(GPIOB , GPIO_Pin_13);
	else if((ch == 2)&&(sta == OFF)) GPIO_SetBits(GPIOB , GPIO_Pin_13);
	else if((ch == 3)&&(sta == ON)) GPIO_ResetBits(GPIOE , GPIO_Pin_2);
	else if((ch == 3)&&(sta == OFF)) GPIO_SetBits(GPIOE , GPIO_Pin_2);
}

void Relay_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE , GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
}

void Relay_Work_Mod_Con(unsigned char ch,unsigned char mod)
{
	unsigned int conBit;
	if((ch>0)&&(ch<7))
	{
		if(ch == 1) conBit = GPIO_Pin_8;
		else if(ch == 2) conBit = GPIO_Pin_9;
		else if(ch == 3) conBit = GPIO_Pin_10;
		else if(ch == 4) conBit = GPIO_Pin_11;
		else if(ch == 5) conBit = GPIO_Pin_12;
		else if(ch == 6) conBit = GPIO_Pin_13;
		
		if(mod == ON)
		{
			GPIO_SetBits(GPIOE,conBit);
			relayWorkSta[ch*2+1] = 1;
		}			
		else 
		{
			GPIO_ResetBits(GPIOE,conBit);
			relayWorkSta[ch*2+1] = 0;
		}
	}
	
}
 
void RelayStatusDatInit()
{
	unsigned char i;
	for(i=0;i<15;i++)
	{
		relayWorkSta[i] = 0;
	}
	relayWorkSta[0] = 0xfa;
	relayWorkSta[1] = 6;
	relayWorkSta[14] = 0xfb;
}
void USART1_SendData(unsigned char dat)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) !=SET);//��ѯֱ�������������	
	USART_SendData(USART1,dat);
}

void msDelay(unsigned int cnt)
{
	delayTimCnt = 0;
	while(delayTimCnt < cnt)
	{
		;
	}
}
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM2_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* ����ʱ�� */
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}

void TIM2_Start(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
void TIM2_Stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);
}

void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		
		
		timMinuteCnt++;
		delayTimCnt++;
		if(timMinuteCnt > 60000)
		{
			timMinuteCnt = 0;
			timMinuteFlag = 1;
		}
		relayDataSendCnt++;
		if(relayDataSendCnt > 2000)
		{
			relayDataSendCnt = 0;
			relayDataSendFlag = 1;
		}
		
		tempTickDetectCnt++;
		if(tempTickDetectCnt > 10000)
		{
			tempTickDetectCnt = 0;
			tempTickDetectFlag = 1;
		}
		
		
		
	}		 	
}
void USART3_IRQHandler(void)
{
	unsigned char ch,i;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		ch = USART3->DR;
		if((ch == 0xfa)&&(tempBcommFlag == 0))
		{
			tempBcommFlag = 1;
			tempBcommCnt = 0;
		}
		else if((tempBcommFlag == 1)&&(tempBcommCnt<12))
		{
			tempBcommDataBuf[tempBcommCnt] = ch;
			tempBcommCnt++;
		}
		else if((tempBcommFlag == 1)&&(ch == 0xfb))
		{
			for(i=0;i<12;i++)
			{
				uarttempDataBufB[i+2] = tempBcommDataBuf[i];
			}
			tempBDataFlag = 1;
			tempBcommFlag = 0;
			tempBcommCnt = 0;
		}
		else
		{
			tempBcommFlag = 0;
			tempBcommCnt = 0;		
		}
			
	}
}
void UART5_IRQHandler(void)
{
	unsigned char ch,i;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) 
	{
		ch = UART5->DR;
		if((ch == 0xfa)&&(tempAcommFlag == 0))
		{
			tempAcommFlag = 1;
			tempAcommCnt = 0;
		}
		else if((tempAcommFlag == 1)&&(tempAcommCnt<12))
		{
			tempAcommDataBuf[tempAcommCnt] = ch;
			tempAcommCnt++;
		}
		else if((tempAcommFlag == 1)&&(ch == 0xfb))
		{
			for(i=0;i<12;i++)
			{
				uarttempDataBufA[i+2] = tempAcommDataBuf[i];
			}

			tempADataFlag = 1;
			tempAcommFlag = 0;
			tempAcommCnt = 0;
		}
		else
		{
			tempAcommFlag = 0;
			tempAcommCnt = 0;		
		}
			
	}
}
void USART1_IRQHandler(void)               
{
	unsigned char ch,i,d;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		ch = USART_ReceiveData(USART1);
		
		if((ch == 'R')&&(uart1CommFlag == 0))
		{
			uart1CommFlag = 1;
		}
		else if((ch == '=')&&(uart1CommFlag == 1))
		{
			uart1CommFlag = 2;
			uart1CommCnt = 0;
		}
		else if((ch == 'A')&&(uart1CommFlag == 2)) //���յ�ͨѶģ����ߵ�ָ��"R=A"
		{
			uart1CommFlag = 0;
			relayAllOffFlag = 1;
		}
		else if((uart1CommFlag == 2)&&(uart1CommCnt <6))
		{
			uart1CommBuf[uart1CommCnt] = ch;
			uart1CommCnt++;
		}
		else if((uart1CommFlag == 2)&&(ch == 'D'))
		{
			uart1CommFlag =  3;
		}
		else if(uart1CommFlag == 3)
		{
			ch = 0xff;  // do nothing
		}
		else
		{
			uart1CommFlag = 0;
		}
		
		d = uart1CommBuf[0] - 0x30;//�̵���ͨ��ֵ��ȡֵ1-6
		if((uart1CommFlag == 3)&&(d > 0)&&(d < 7))
		{
			uartWorkMode[d-1] = 1;
			for(i=0;i<5;i++)
			{
				uartWorkDat[d-1][i] = uart1CommBuf[i+1];
			}

			uart1CommFlag = 0;
			
		}
		
	} 
} 
/*******************************/
