#include "usart.h"

#define NEED_UART1  
///////////////////////////////////////////////////////////////////////////////////////
/**
  * @名称  int fputc(int ch, FILE *f)
  * @说明  重定义fputc函数,加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
  * @参数  无
  *    
  *     @参数
  *     @参数
  *     
  * @返回值 无
  */
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{     
#ifdef NEED_UART1
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;     
#endif
	
#ifdef NEED_UART2
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;
#endif
	return ch;
}
//////////////////////////////////////////////////////////////////////////////////////////
//DataSourceStruct	DataSource;
//RecStateStruct 		RecState = IsNone;

HandleTypeStruct UartHandleType = IsNoneType;

static u8	Rx_Buff[RX_BUFF_LEN] = {0};		//收到的字节
static u8  	Tx_Buff[TX_BUFF_LEN] = {0};		//发送的字节
static u16 	Rx_CNT = 0; 					//接收的长度   
static u16 	Tx_CNT = 0; 					//发送长度

void DMA_TX_Init(u8 com,u8 *Sbuffaddr);
void RS485M_Uart2_SendData(u8 *buf,u8 len);
 

/*****************************************
  * @名称  void UartInit(void)
  * @说明  串口初始化
  * @参数  无
  *    
  *     @参数
  *     @参数
  *     
  * @返回值 无
******************************************/
#define NEED_DEBUG
void Usart_X_Init(void)
{	
	Usart_Init(1, BAUDRATE,USART_Parity_No,USART_StopBits_1);	//串口1DMA初始化
//	DMA_TX_Init(1,Tx_Buff);										//串口1DMA发送初始化
//	DMA_RX_Init(1,Rx_Buff);										//串口1DMA接收初始化

#ifdef NEED_DEBUG
	UsartDmaXSendData(1,"uart1configok\r\n",strlen("uart1configok\r\n"));
#endif
}


/**
  * @名称  void Usart_Init(u8 com,u32 bound,u16 Parity,u16 stopbit)
  * @说明  串口初始化
  * @参数  无
  *    
  *     @参数  串口号
  *     @参数  波特率
  *     @参数  校验位
  *     @参数  停止位
  *     
  * @返回值 无
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
	u8 compriority;    //优先级

	switch(com)	//根据串口号选择不同的设置值
	{
		case 1:  							//如果是串口1
			GPIO_Pin_Tx = GPIO_Pin_9;		//设置发送引脚
			GPIO_Pin_Rx = GPIO_Pin_10;		//设置接收引脚
			USART = USART1;	  				//设置为串口1
			GPIO  = GPIOA;					//设置GPIO组为A
			USART_IRQChannel = USART1_IRQn;	//设置串口1中断
			compriority = 0;				//中断优先级
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);		//使能相应的时钟
		break;
		
		case 2:	//如果是串口2
			GPIO_Pin_Tx = GPIO_Pin_2;			//设置发送引脚
			GPIO_Pin_Rx = GPIO_Pin_3;			//设置接收引脚
			USART = USART2;								//设置为串口2
			GPIO  = GPIOA;								//设置GPIO组
			USART_IRQChannel = USART2_IRQn;		//设置串口2中断
			compriority = 1;			//中断优先级
			//使能相应的时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	
		break;
		
		case 3:	//如果是串口3
			GPIO_Pin_Tx = GPIO_Pin_10;		//设置发送引脚
			GPIO_Pin_Rx = GPIO_Pin_11;		//设置接收引脚
			USART = USART3;								//设置为串口3
			GPIO  = GPIOB;								//设置GPIO组
			USART_IRQChannel = USART3_IRQn;	//设置串口中断
			compriority = 2;			//中断优先级
			/****************USART3时钟使能*****************/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			break;
		
		default:
			break;
	}	

	//GPIO引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Tx;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIO,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Rx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO,&GPIO_InitStructure);

	/****************中断配置*****************/
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQChannel;										//USART1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = compriority ;		 //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					   					 //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						    					//IRQ中断被使能
	NVIC_Init(&NVIC_InitStructure);									        									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	/***USART1参数的配置 波特率19200 8位数据位 1位停止位 无校验 无硬件控制流 发送和接收***/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = (Parity == USART_Parity_No)?(USART_WordLength_8b):(USART_WordLength_9b);
	USART_InitStructure.USART_StopBits = stopbit;
	USART_InitStructure.USART_Parity = Parity;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART,&USART_InitStructure);								//初始化串口

	USART_ITConfig(USART, USART_IT_IDLE, ENABLE);         //??????
	USART_ITConfig(USART,USART_IT_TC,DISABLE);        		//??????
	USART_ITConfig(USART,USART_IT_RXNE,DISABLE);    			//??????

	USART_Cmd(USART,ENABLE);														  //????
	USART_ClearFlag(USART, USART_FLAG_TC);								//???????????
}	


/**
  * @名称  void DMA_TX_Init(u8 com,u8 *Sbuffaddr)
  * @说明  DMA发送初始化
  * @参数  无
  *    
  *     @参数  串口号
  *     @参数  发送缓存
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
void DMA_TX_Init(u8 com,u8 *Sbuffaddr)
{
	NVIC_InitTypeDef 		NVIC_InitStructure;
	DMA_InitTypeDef 		DMA_InitStructure;
	
	USART_TypeDef* USART;
	DMA_Channel_TypeDef* DMA_Channel_Tx;
	u8 DMA_IRQChannel_Tx;
	u8 compriority;    //优先级	
	
	switch(com)	//根据串口号选择不同的设置值
	{
		case 1:  //如果是串口1

			USART = USART1;	  //设置为串口1
			DMA_Channel_Tx = DMA1_Channel4;  //DMA发送通道为4
			DMA_IRQChannel_Tx = DMA1_Channel4_IRQn;	//设置DMA发送中断为4
		  compriority = 0;		//中断优先级
			break;
		case 2:	//如果是串口2
			USART = USART2;								//设置为串口2
			DMA_Channel_Tx = DMA1_Channel7;		//设置DMA发送通道为7
			DMA_IRQChannel_Tx = DMA1_Channel7_IRQn;  //设置DMA中断
		  compriority = 1;			//中断优先级
			break;
		case 3:	//如果是串口3
			USART = USART3;								//设置为串口3
			DMA_Channel_Tx = DMA1_Channel2;		//设置DMA发送通道
			DMA_IRQChannel_Tx = DMA1_Channel2_IRQn;	//设置DMA发送中断
		  compriority = 2;			//中断优先级
			break;
		default:
			break;
	}	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			//使能DMA1时钟
  
  /******************初始化DMA发送************************/
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
	
/*********************发送DMA***********************/
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
  * @名称  void DMA_RX_Init(u8 com,u8 *Rbuffaddr)
  * @说明  DMA接收初始化
  * @参数  无
  *    
  *     @参数  串口号
  *     @参数  接收缓存
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
void DMA_RX_Init(u8 com,u8 *Rbuffaddr)
{
	NVIC_InitTypeDef 		NVIC_InitStructure;
	DMA_InitTypeDef 		DMA_InitStructure;
	
	USART_TypeDef* USART;
	DMA_Channel_TypeDef* DMA_Channel_Rx;	
	u8 DMA_IRQChannel_Rx;								
	u8 compriority;    //优先级
	
		switch(com)	//根据串口号选择不同的设置值
	{
		case 1:  //如果是串口1
			USART = USART1;	  //设置为串口1
			DMA_Channel_Rx = DMA1_Channel5;  //DMA接收通道5
			DMA_IRQChannel_Rx = DMA1_Channel5_IRQn;  //设置DMA接收中断为5
		  compriority = 0;		//中断优先级
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);		//使能相应的时钟
			break;
		case 2:	//如果是串口2
			USART = USART2;								//设置为串口2
			DMA_Channel_Rx = DMA1_Channel6;		//设置DMA接收通道为6
			DMA_IRQChannel_Rx = DMA1_Channel6_IRQn;  //设置DMA接收中断为6
		  compriority = 1;			//中断优先级
			//使能相应的时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	
			break;
		case 3:	//如果是串口3
			USART = USART3;								//设置为串口3
			DMA_Channel_Rx = DMA1_Channel3;		//设置DMA发送通道
			DMA_IRQChannel_Rx = DMA1_Channel3_IRQn;	//设置DMA发送中断	
		  compriority = 2;			//中断优先级
				/****************USART3时钟使能*****************/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			break;
		default:
			break;
	}	

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			//使能DMA1时钟
	
		DMA_DeInit(DMA_Channel_Rx);												//DMA接收通道配置
		DMA_StructInit(&DMA_InitStructure);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART->DR;  //外设地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rbuffaddr;				//内存地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//dma传输方向 单向
		DMA_InitStructure.DMA_BufferSize = RX_BUFF_LEN;														//DMA缓冲长度
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//设置DMA的外设递增模式，一个外设
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//设置DMA的内存递增模式
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //外设数据字长
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		//内存数据字长
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;											//设置DMA的传输模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//设置DMA的优先级别
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;												//设置DMA的2个memory中的变量相互访问
		DMA_Init(DMA_Channel_Rx, &DMA_InitStructure);											//初始化DMA
		
		DMA_Cmd(DMA_Channel_Rx, ENABLE);																//使能通道5
		USART_DMACmd(USART, USART_DMAReq_Rx, ENABLE);											//采用DMA方式接受

		NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQChannel_Rx;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = compriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @名称  void RTS_GpioInit(void)
  * @说明  RS485控制引脚
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
void RTS_GpioInit(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
}	

 /**
  * @名称  void DMA1_Channel4_IRQHandler(void)
  * @说明  串口1DMA发送中断处理
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
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
  * @名称  void DMA1_Channel5_IRQHandler(void)
  * @说明  串口1DMA接收中断处理函数
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
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
  * @名称  void DMA1_Channel7_IRQHandler(void)
  * @说明  串口2DMA发送中断处理函数
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
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
		//PBout(9) = 0;          //RTS引脚置低
  }
	
}

 /**
  * @名称  void DMA1_Channel6_IRQHandler(void)
  * @说明  串口2DMA接收中断处理函数
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
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
  * @名称  void USART2_IRQHandler(void)
  * @说明  串口2接收中断处理函数
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
#ifdef NEED_UART1
void USART1_IRQHandler(void)
{
	u16 temp;
	//判断是否真的空闲帧来了
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{
		temp = USART1->SR;   				//先读SR,然后DR才能清除
		temp = USART1->DR; 					//清USART_IT_IDLE标志

		DMA_Cmd(DMA1_Channel5,DISABLE);   	//关闭DMA接收，防止期间还有数据过来
		//计算接收的字节数
		Rx_CNT = RX_BUFF_LEN - DMA_GetCurrDataCounter(DMA1_Channel5); 
		UartHandleType = IsRecOkType;		
	}	
}
#endif

 /**
  * @名称  void USART2_IRQHandler(void)
  * @说明  串口2接收中断处理函数
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
#ifdef NEED_UART2
void USART2_IRQHandler(void)
{
	 
	 u16 len = 0;   
       
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	 {  
		len = USART2->SR;  
		len = USART2->DR; //清USART_IT_IDLE标志  
		DMA_Cmd(DMA1_Channel6,DISABLE);  

		Rx_CNT = RX_BUFF_LEN - DMA1_Channel6->CNDTR; 
		UartHandleType = IsRecOkType;		

		//DMA1_Channel6->CNDTR = RX_BUFF_LEN; //设置传输数据长度  此处不能打开，需要处理完后才能打开接收
		//DMA_Cmd(DMA1_Channel6,ENABLE);//打开DMA    
    }   

}
#endif

/**
  * @名称  void UART4_IRQHandler(void)
  * @说明  串口4接收中断处理函数
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
*/
#ifdef NEED_UART4
void UART4_IRQHandler(void)
{
	u16 temp;
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)  	//判断是否真的空闲帧来了
	{
		temp = UART4->SR;   								//先读SR,然后DR才能清除
		temp = UART4->DR; 									//清USART_IT_IDLE标志
		DMA_Cmd(DMA2_Channel3,DISABLE);  							 	//关闭DMA接收，防止期间还有数据过来
		Rx_CNT = RX_BUFF_LEN - DMA_GetCurrDataCounter(DMA2_Channel3);	//计算接收的字节数
		DataSource = IsUart;
		RecState = IsRecFinish;											//表示接收完成
	}	
}
#endif


/**********************************************************
	* @名称  void Usart1Dma_Send_Data(u8 *buf,u16 len)
	* @说明  串口1DMA发送
	* @参数  无
	*    
	*     @参数  
	*     @参数   
	*     @参数 
	*     @参数  
	*     
	* @返回值 无
***********************************************************/
#ifdef NEED_UART1
static void Usart1DmaSendData(u8 *buf,u16 len)
{
	DMA_Cmd(DMA1_Channel4,DISABLE);  			//关闭DMA
	memcpy(Tx_Buff,buf,len);					//填充数据到1发送缓存区
	USART_ClearFlag(USART1,USART_FLAG_TC);		//清除发送完成标志位

	delay_us(500);								//延时500us  

	DMA_SetCurrDataCounter(DMA1_Channel4,len);	//设置DMA发送缓存区大小
	DMA_Cmd(DMA1_Channel4, ENABLE);				//使能DMA1通道4
} 
#endif


/**
  * @名称  void Usart2Dma_Send_Data(u8 *buf,u8 len)
  * @说明  串口2DMA发送
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
*/
#ifdef NEED_UART2
static void Usart2DmaSendData(u8 *buf,u16 len)
{
	
	DMA_Cmd(DMA1_Channel7,DISABLE);		//关闭DMA
	delay_us(500);	
	//memset(Tx_Buff,0,TX_BUFF_LEN);
	memcpy(Tx_Buff,buf,len);

#if 0
	/***************************************************/
	DMA1_Channel7->CCR &= ~DMA_CCR1_EN;	   
	DMA1->IFCR = DMA1_FLAG_GL7;
	DMA1_Channel7->CNDTR = len;                  //  重新赋值计数值
	DMA1_Channel7->CMAR = (uint32_t)Tx_Buff;	 //设置内存地址
	DMA1_Channel7->CCR |= DMA_CCR1_EN;
	/***************************************************/	
#else
	DMA_SetCurrDataCounter(DMA1_Channel7,len);
	DMA_Cmd(DMA1_Channel7, ENABLE);
#endif
}
#endif


 /**
  * @名称  void Usart2Dma_Send_Data(u8 *buf,u8 len)
  * @说明  串口2DMA发送
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
#ifdef NEED_UART1
void RS485S_Uart1_SendData(u8 *buf,u8 len)
{
	u8 t;

//	RS485_TX_EN = 1;
	delay_us(500);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	for(t=0;t<len;t++)										   //循环发送数据
	{		   
			USART1->DR=buf[t];
			while((USART1->SR&0X40)==0);					//等待发送结束	
	}
	
//	RS485_TX_EN = 0;
	
}
#endif

 /**
  * @名称  void Usart2Dma_Send_Data(u8 *buf,u8 len)
  * @说明  串口2DMA发送
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
#if 1
void RS485M_Uart2_SendData(u8 *buf,u8 len)
{
	u8 t;
	//RS485M2_TX_EN =1;
	delay_us(500);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	for(t=0;t<len;t++)										   //循环发送数据
	{		   
		USART2->DR=buf[t];
		while((USART2->SR&0X40)==0);					//等待发送结束	
	}
				 
  //RS485M2_TX_EN = 0;
	
}
#endif

 /**
  * @名称  void RS485S_Uart4_SendData(u8 *buf,u8 len)
  * @说明  串口2DMA发送
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
  */
#ifdef NEED_UART4
void RS485S_Uart4_SendData(u8 *buf,u8 len)
{
	u8 t;
	//RS485_TX_EN = 1;
	delay_us(500);
	USART_ClearFlag(UART4,USART_FLAG_TC);
	for(t=0;t<len;t++)										   //循环发送数据
	{		   
			UART4->DR=buf[t];
			while((UART4->SR&0X40)==0);					//等待发送结束	
	}
	//RS485_TX_EN = 0;
	
}
#endif
 /**
  * @名称  void EnableDmaRec(u8 com)
  * @说明  使能DMA发送
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
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
  * @名称  void EnableDmaRec(u8 com)
  * @说明  使能DMA发送
  * @参数  无
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值：接收数据的总长度
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
  * @名称  void UsartDmaXSendData(u8 com,u8 *buf,u16 len)
  * @说明  UartDma发送
  * @参数    
  *    
  *     @参数  
  *     @参数   
  *     @参数 
  *     @参数  
  *     
  * @返回值 无
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
