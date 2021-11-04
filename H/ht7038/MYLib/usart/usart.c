#include "usart.h"

#define NEED_UART1  
///////////////////////////////////////////////////////////////////////////////////////
/**
  * @����  int fputc(int ch, FILE *f)
  * @˵��  �ض���fputc����,�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
  * @����  ��
  *    
  *     @����
  *     @����
  *     
  * @����ֵ ��
  */
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{     
#ifdef NEED_UART1
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;     
#endif
	
#ifdef NEED_UART2
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;
#endif
	return ch;
}
//////////////////////////////////////////////////////////////////////////////////////////
//DataSourceStruct	DataSource;
//RecStateStruct 		RecState = IsNone;

HandleTypeStruct UartHandleType = IsNoneType;

static u8	Rx_Buff[RX_BUFF_LEN] = {0};		//�յ����ֽ�
static u8  	Tx_Buff[TX_BUFF_LEN] = {0};		//���͵��ֽ�
static u16 	Rx_CNT = 0; 					//���յĳ���   
static u16 	Tx_CNT = 0; 					//���ͳ���

void DMA_TX_Init(u8 com,u8 *Sbuffaddr);
void RS485M_Uart2_SendData(u8 *buf,u8 len);
 

/*****************************************
  * @����  void UartInit(void)
  * @˵��  ���ڳ�ʼ��
  * @����  ��
  *    
  *     @����
  *     @����
  *     
  * @����ֵ ��
******************************************/
#define NEED_DEBUG
void Usart_X_Init(void)
{	
	Usart_Init(1, BAUDRATE,USART_Parity_No,USART_StopBits_1);	//����1DMA��ʼ��
//	DMA_TX_Init(1,Tx_Buff);										//����1DMA���ͳ�ʼ��
//	DMA_RX_Init(1,Rx_Buff);										//����1DMA���ճ�ʼ��

#ifdef NEED_DEBUG
	UsartDmaXSendData(1,"uart1configok\r\n",strlen("uart1configok\r\n"));
#endif
}


/**
  * @����  void Usart_Init(u8 com,u32 bound,u16 Parity,u16 stopbit)
  * @˵��  ���ڳ�ʼ��
  * @����  ��
  *    
  *     @����  ���ں�
  *     @����  ������
  *     @����  У��λ
  *     @����  ֹͣλ
  *     
  * @����ֵ ��
*/
void Usart_Init(u8 com,u32 bound,u16 Parity,u16 stopbit)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;

	u16 GPIO_Pin_Rx, GPIO_Pin_Tx;		
	USART_TypeDef* USART;									
	GPIO_TypeDef * GPIO;																
	u8 USART_IRQChannel;
	u8 compriority;    //���ȼ�

	switch(com)	//���ݴ��ں�ѡ��ͬ������ֵ
	{
		case 1:  							//����Ǵ���1
			GPIO_Pin_Tx = GPIO_Pin_9;		//���÷�������
			GPIO_Pin_Rx = GPIO_Pin_10;		//���ý�������
			USART = USART1;	  				//����Ϊ����1
			GPIO  = GPIOA;					//����GPIO��ΪA
			USART_IRQChannel = USART1_IRQn;	//���ô���1�ж�
			compriority = 0;				//�ж����ȼ�
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);		//ʹ����Ӧ��ʱ��
		break;
		
		case 2:	//����Ǵ���2
			GPIO_Pin_Tx = GPIO_Pin_2;			//���÷�������
			GPIO_Pin_Rx = GPIO_Pin_3;			//���ý�������
			USART = USART2;								//����Ϊ����2
			GPIO  = GPIOA;								//����GPIO��
			USART_IRQChannel = USART2_IRQn;		//���ô���2�ж�
			compriority = 1;			//�ж����ȼ�
			//ʹ����Ӧ��ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	
		break;
		
		case 3:	//����Ǵ���3
			GPIO_Pin_Tx = GPIO_Pin_10;		//���÷�������
			GPIO_Pin_Rx = GPIO_Pin_11;		//���ý�������
			USART = USART3;								//����Ϊ����3
			GPIO  = GPIOB;								//����GPIO��
			USART_IRQChannel = USART3_IRQn;	//���ô����ж�
			compriority = 2;			//�ж����ȼ�
			/****************USART3ʱ��ʹ��*****************/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			break;
		
		default:
			break;
	}	

	//GPIO���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Tx;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIO,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Rx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO,&GPIO_InitStructure);

	/****************�ж�����*****************/
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQChannel;										//USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = compriority ;		 //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					   					 //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						    					//IRQ�жϱ�ʹ��
	NVIC_Init(&NVIC_InitStructure);									        									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	/***USART1���������� ������19200 8λ����λ 1λֹͣλ ��У�� ��Ӳ�������� ���ͺͽ���***/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = (Parity == USART_Parity_No)?(USART_WordLength_8b):(USART_WordLength_9b);
	USART_InitStructure.USART_StopBits = stopbit;
	USART_InitStructure.USART_Parity = Parity;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART,&USART_InitStructure);								//��ʼ������

	USART_ITConfig(USART, USART_IT_IDLE, ENABLE);         //??????
	USART_ITConfig(USART,USART_IT_TC,DISABLE);        		//??????
	USART_ITConfig(USART,USART_IT_RXNE,DISABLE);    			//??????

	USART_Cmd(USART,ENABLE);														  //????
	USART_ClearFlag(USART, USART_FLAG_TC);								//???????????
}	


/**
  * @����  void DMA_TX_Init(u8 com,u8 *Sbuffaddr)
  * @˵��  DMA���ͳ�ʼ��
  * @����  ��
  *    
  *     @����  ���ں�
  *     @����  ���ͻ���
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void DMA_TX_Init(u8 com,u8 *Sbuffaddr)
{
	NVIC_InitTypeDef 		NVIC_InitStructure;
	DMA_InitTypeDef 		DMA_InitStructure;
	
	USART_TypeDef* USART;
	DMA_Channel_TypeDef* DMA_Channel_Tx;
	u8 DMA_IRQChannel_Tx;
	u8 compriority;    //���ȼ�	
	
	switch(com)	//���ݴ��ں�ѡ��ͬ������ֵ
	{
		case 1:  //����Ǵ���1

			USART = USART1;	  //����Ϊ����1
			DMA_Channel_Tx = DMA1_Channel4;  //DMA����ͨ��Ϊ4
			DMA_IRQChannel_Tx = DMA1_Channel4_IRQn;	//����DMA�����ж�Ϊ4
		  compriority = 0;		//�ж����ȼ�
			break;
		case 2:	//����Ǵ���2
			USART = USART2;								//����Ϊ����2
			DMA_Channel_Tx = DMA1_Channel7;		//����DMA����ͨ��Ϊ7
			DMA_IRQChannel_Tx = DMA1_Channel7_IRQn;  //����DMA�ж�
		  compriority = 1;			//�ж����ȼ�
			break;
		case 3:	//����Ǵ���3
			USART = USART3;								//����Ϊ����3
			DMA_Channel_Tx = DMA1_Channel2;		//����DMA����ͨ��
			DMA_IRQChannel_Tx = DMA1_Channel2_IRQn;	//����DMA�����ж�
		  compriority = 2;			//�ж����ȼ�
			break;
		default:
			break;
	}	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			//ʹ��DMA1ʱ��
  
  /******************��ʼ��DMA����************************/
  DMA_DeInit(DMA_Channel_Tx);																
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Sbuffaddr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = TX_BUFF_LEN;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA_Channel_Tx, &DMA_InitStructure); 
	
/*********************����DMA***********************/
  NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQChannel_Tx;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = compriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);	//????
	
	/******************************************************/
  DMA_ITConfig(DMA_Channel_Tx, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA_Channel_Tx, DMA_IT_TE, ENABLE);
  USART_DMACmd(USART, USART_DMAReq_Tx, ENABLE);
  DMA_Cmd(DMA_Channel_Tx, DISABLE);
}	


 /**
  * @����  void DMA_RX_Init(u8 com,u8 *Rbuffaddr)
  * @˵��  DMA���ճ�ʼ��
  * @����  ��
  *    
  *     @����  ���ں�
  *     @����  ���ջ���
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void DMA_RX_Init(u8 com,u8 *Rbuffaddr)
{
	NVIC_InitTypeDef 		NVIC_InitStructure;
	DMA_InitTypeDef 		DMA_InitStructure;
	
	USART_TypeDef* USART;
	DMA_Channel_TypeDef* DMA_Channel_Rx;	
	u8 DMA_IRQChannel_Rx;								
	u8 compriority;    //���ȼ�
	
		switch(com)	//���ݴ��ں�ѡ��ͬ������ֵ
	{
		case 1:  //����Ǵ���1
			USART = USART1;	  //����Ϊ����1
			DMA_Channel_Rx = DMA1_Channel5;  //DMA����ͨ��5
			DMA_IRQChannel_Rx = DMA1_Channel5_IRQn;  //����DMA�����ж�Ϊ5
		  compriority = 0;		//�ж����ȼ�
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);		//ʹ����Ӧ��ʱ��
			break;
		case 2:	//����Ǵ���2
			USART = USART2;								//����Ϊ����2
			DMA_Channel_Rx = DMA1_Channel6;		//����DMA����ͨ��Ϊ6
			DMA_IRQChannel_Rx = DMA1_Channel6_IRQn;  //����DMA�����ж�Ϊ6
		  compriority = 1;			//�ж����ȼ�
			//ʹ����Ӧ��ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	
			break;
		case 3:	//����Ǵ���3
			USART = USART3;								//����Ϊ����3
			DMA_Channel_Rx = DMA1_Channel3;		//����DMA����ͨ��
			DMA_IRQChannel_Rx = DMA1_Channel3_IRQn;	//����DMA�����ж�	
		  compriority = 2;			//�ж����ȼ�
				/****************USART3ʱ��ʹ��*****************/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			break;
		default:
			break;
	}	

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			//ʹ��DMA1ʱ��
	
		DMA_DeInit(DMA_Channel_Rx);												//DMA����ͨ������
		DMA_StructInit(&DMA_InitStructure);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART->DR;  //�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rbuffaddr;				//�ڴ��ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//dma���䷽�� ����
		DMA_InitStructure.DMA_BufferSize = RX_BUFF_LEN;														//DMA���峤��
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����DMA���������ģʽ��һ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//����DMA���ڴ����ģʽ
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���������ֳ�
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		//�ڴ������ֳ�
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;											//����DMA�Ĵ���ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//����DMA�����ȼ���
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;												//����DMA��2��memory�еı����໥����
		DMA_Init(DMA_Channel_Rx, &DMA_InitStructure);											//��ʼ��DMA
		
		DMA_Cmd(DMA_Channel_Rx, ENABLE);																//ʹ��ͨ��5
		USART_DMACmd(USART, USART_DMAReq_Rx, ENABLE);											//����DMA��ʽ����

		NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQChannel_Rx;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = compriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @����  void RTS_GpioInit(void)
  * @˵��  RS485��������
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void RTS_GpioInit(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
}	

 /**
  * @����  void DMA1_Channel4_IRQHandler(void)
  * @˵��  ����1DMA�����жϴ���
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void DMA1_Channel4_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_IT_TE4) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TE4);
    return;
  }

  if(DMA_GetFlagStatus(DMA1_IT_TC4) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);	
	delay_us(1000);
  }
	
}

 /**
  * @����  void DMA1_Channel5_IRQHandler(void)
  * @˵��  ����1DMA�����жϴ�����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void DMA1_Channel5_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_IT_TE5) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TE5);
    return;
  }

  if(DMA_GetFlagStatus(DMA1_IT_TC5) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TC5);
    DMA_Cmd(DMA1_Channel5, DISABLE);
  }
	
}

 /**
  * @����  void DMA1_Channel7_IRQHandler(void)
  * @˵��  ����2DMA�����жϴ�����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void DMA1_Channel7_IRQHandler(void)
{
	 if(DMA_GetFlagStatus(DMA1_IT_TE7) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TE7);
    return;
  }

  if(DMA_GetFlagStatus(DMA1_IT_TC7) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TC7);
    DMA_Cmd(DMA1_Channel7, DISABLE);
		delay_us(1000);
		//PBout(9) = 0;          //RTS�����õ�
  }
	
}

 /**
  * @����  void DMA1_Channel6_IRQHandler(void)
  * @˵��  ����2DMA�����жϴ�����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void DMA1_Channel6_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_IT_TE6) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TE6);
    return;
  }

  if(DMA_GetFlagStatus(DMA1_IT_TC6) != RESET)
  {
    DMA_ClearITPendingBit(DMA1_IT_TC6);
    DMA_Cmd(DMA1_Channel6, DISABLE);
  }
	
}

 /**
  * @����  void USART2_IRQHandler(void)
  * @˵��  ����2�����жϴ�����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
#ifdef NEED_UART1
void USART1_IRQHandler(void)
{
	u16 temp;
	//�ж��Ƿ���Ŀ���֡����
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{
		temp = USART1->SR;   				//�ȶ�SR,Ȼ��DR�������
		temp = USART1->DR; 					//��USART_IT_IDLE��־

		DMA_Cmd(DMA1_Channel5,DISABLE);   	//�ر�DMA���գ���ֹ�ڼ仹�����ݹ���
		//������յ��ֽ���
		Rx_CNT = RX_BUFF_LEN - DMA_GetCurrDataCounter(DMA1_Channel5); 
		UartHandleType = IsRecOkType;		
	}	
}
#endif

 /**
  * @����  void USART2_IRQHandler(void)
  * @˵��  ����2�����жϴ�����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
#ifdef NEED_UART2
void USART2_IRQHandler(void)
{
	 
	 u16 len = 0;   
       
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	 {  
		len = USART2->SR;  
		len = USART2->DR; //��USART_IT_IDLE��־  
		DMA_Cmd(DMA1_Channel6,DISABLE);  

		Rx_CNT = RX_BUFF_LEN - DMA1_Channel6->CNDTR; 
		UartHandleType = IsRecOkType;		

		//DMA1_Channel6->CNDTR = RX_BUFF_LEN; //���ô������ݳ���  �˴����ܴ򿪣���Ҫ���������ܴ򿪽���
		//DMA_Cmd(DMA1_Channel6,ENABLE);//��DMA    
    }   

}
#endif

/**
  * @����  void UART4_IRQHandler(void)
  * @˵��  ����4�����жϴ�����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
*/
#ifdef NEED_UART4
void UART4_IRQHandler(void)
{
	u16 temp;
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)  	//�ж��Ƿ���Ŀ���֡����
	{
		temp = UART4->SR;   								//�ȶ�SR,Ȼ��DR�������
		temp = UART4->DR; 									//��USART_IT_IDLE��־
		DMA_Cmd(DMA2_Channel3,DISABLE);  							 	//�ر�DMA���գ���ֹ�ڼ仹�����ݹ���
		Rx_CNT = RX_BUFF_LEN - DMA_GetCurrDataCounter(DMA2_Channel3);	//������յ��ֽ���
		DataSource = IsUart;
		RecState = IsRecFinish;											//��ʾ�������
	}	
}
#endif


/**********************************************************
	* @����  void Usart1Dma_Send_Data(u8 *buf,u16 len)
	* @˵��  ����1DMA����
	* @����  ��
	*    
	*     @����  
	*     @����   
	*     @���� 
	*     @����  
	*     
	* @����ֵ ��
***********************************************************/
#ifdef NEED_UART1
static void Usart1DmaSendData(u8 *buf,u16 len)
{
	DMA_Cmd(DMA1_Channel4,DISABLE);  			//�ر�DMA
	memcpy(Tx_Buff,buf,len);					//������ݵ�1���ͻ�����
	USART_ClearFlag(USART1,USART_FLAG_TC);		//���������ɱ�־λ

	delay_us(500);								//��ʱ500us  

	DMA_SetCurrDataCounter(DMA1_Channel4,len);	//����DMA���ͻ�������С
	DMA_Cmd(DMA1_Channel4, ENABLE);				//ʹ��DMA1ͨ��4
} 
#endif


/**
  * @����  void Usart2Dma_Send_Data(u8 *buf,u8 len)
  * @˵��  ����2DMA����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
*/
#ifdef NEED_UART2
static void Usart2DmaSendData(u8 *buf,u16 len)
{
	
	DMA_Cmd(DMA1_Channel7,DISABLE);		//�ر�DMA
	delay_us(500);	
	//memset(Tx_Buff,0,TX_BUFF_LEN);
	memcpy(Tx_Buff,buf,len);

#if 0
	/***************************************************/
	DMA1_Channel7->CCR &= ~DMA_CCR1_EN;	   
	DMA1->IFCR = DMA1_FLAG_GL7;
	DMA1_Channel7->CNDTR = len;                  //  ���¸�ֵ����ֵ
	DMA1_Channel7->CMAR = (uint32_t)Tx_Buff;	 //�����ڴ��ַ
	DMA1_Channel7->CCR |= DMA_CCR1_EN;
	/***************************************************/	
#else
	DMA_SetCurrDataCounter(DMA1_Channel7,len);
	DMA_Cmd(DMA1_Channel7, ENABLE);
#endif
}
#endif


 /**
  * @����  void Usart2Dma_Send_Data(u8 *buf,u8 len)
  * @˵��  ����2DMA����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
#ifdef NEED_UART1
void RS485S_Uart1_SendData(u8 *buf,u8 len)
{
	u8 t;

//	RS485_TX_EN = 1;
	delay_us(500);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	for(t=0;t<len;t++)										   //ѭ����������
	{		   
			USART1->DR=buf[t];
			while((USART1->SR&0X40)==0);					//�ȴ����ͽ���	
	}
	
//	RS485_TX_EN = 0;
	
}
#endif

 /**
  * @����  void Usart2Dma_Send_Data(u8 *buf,u8 len)
  * @˵��  ����2DMA����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
#if 1
void RS485M_Uart2_SendData(u8 *buf,u8 len)
{
	u8 t;
	//RS485M2_TX_EN =1;
	delay_us(500);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	for(t=0;t<len;t++)										   //ѭ����������
	{		   
		USART2->DR=buf[t];
		while((USART2->SR&0X40)==0);					//�ȴ����ͽ���	
	}
				 
  //RS485M2_TX_EN = 0;
	
}
#endif

 /**
  * @����  void RS485S_Uart4_SendData(u8 *buf,u8 len)
  * @˵��  ����2DMA����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
#ifdef NEED_UART4
void RS485S_Uart4_SendData(u8 *buf,u8 len)
{
	u8 t;
	//RS485_TX_EN = 1;
	delay_us(500);
	USART_ClearFlag(UART4,USART_FLAG_TC);
	for(t=0;t<len;t++)										   //ѭ����������
	{		   
			UART4->DR=buf[t];
			while((UART4->SR&0X40)==0);					//�ȴ����ͽ���	
	}
	//RS485_TX_EN = 0;
	
}
#endif
 /**
  * @����  void EnableDmaRec(u8 com)
  * @˵��  ʹ��DMA����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void EnableDmaRec(u8 com)
{
	memset(Rx_Buff,0,RX_BUFF_LEN);
	
	switch(com)
	{
		case 1:
			DMA1_Channel5->CNDTR = RX_BUFF_LEN;				
			DMA_Cmd(DMA1_Channel5,ENABLE);
			break;
		
		case 2:
			DMA1_Channel6->CNDTR = RX_BUFF_LEN;  
			DMA_Cmd(DMA1_Channel6,ENABLE);  
			break;
		
		case 3:
			break;
		
		case 4:
			break;
	}
}


/**
  * @����  void EnableDmaRec(u8 com)
  * @˵��  ʹ��DMA����
  * @����  ��
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ���������ݵ��ܳ���
*/
u16 GetUartData(u8 *buffer)
{
	if(Rx_CNT< 128)
	{
		memcpy(buffer,Rx_Buff,Rx_CNT);
		return Rx_CNT;
	}
	else
		return 0;
}


 /**
  * @����  void UsartDmaXSendData(u8 com,u8 *buf,u16 len)
  * @˵��  UartDma����
  * @����    
  *    
  *     @����  
  *     @����   
  *     @���� 
  *     @����  
  *     
  * @����ֵ ��
  */
void UsartDmaXSendData(u8 com,u8 *buf,u16 len)
{
	switch(com)
	{
		case 1:
			Usart1DmaSendData(buf,len);
		break;
		
		case 2:
	#ifdef NEED_UART2
		Usart2DmaSendData(buf,len);
	#endif
		break;
	}
}
