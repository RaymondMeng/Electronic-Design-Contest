#include "ht7038.h"


uint8_t	APhaseReceBuf[PACK_SIZE] = {0};							//A相数据采集缓存
uint8_t	BPhaseReceBuf[PACK_SIZE] = {0};							//B相数据采集缓存

uint8_t 	EnableCorrectRegBuf[PACK_SIZE]   = {0x00,0x00,0x5a};	//选择校表寄存器
uint8_t 	EnableMeterRegBuf[PACK_SIZE]     = {0x00,0x00,0x5b};	//选择计量寄存器

//校表寄存器的设置值(该类寄存器的字长为2Byte，故要设置的值写在低两字节中)
uint8_t 	ModeConfigBuf[PACK_SIZE] 		 = {0x00,0x89,0xfe};	//设置电压/电流ADC通道
uint8_t 	AnalogModuleConfigBuf[PACK_SIZE] = {0x00,0x35,0x21};	//设置高通滤波(关闭电压&&电流通道数字高通滤波器)

//static void SetHT7038Function(void);


static void HT7038_IO_Init(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|
							RCC_APB2Periph_GPIOB, ENABLE );
	//片选引脚
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;			//HT7038模块片选引脚
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		//时速50MHz
    GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;		//推挽输出
    GPIO_Init( SPI_CS_PORT, &GPIO_InitStructure );			//根据指定参数初始化
    GPIO_SetBits(SPI_CS_PORT,GPIO_Pin_4);
	
	//复位引脚
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;			//HT7038模块复位引脚
    GPIO_Init( SPI_RST_PORT, &GPIO_InitStructure );			//根据指定参数初始化
}


/************************************
	*功能描述：	设置RST引脚状态
	*入口参数：	状态值(0|1)
	*返回值：	无
*************************************/
static void HT7038ResetSet(uint8_t state)
{
    if( state == ENABLE )
    {
		RST_L();			//将复位引脚置低
		//HT7038_RST_PIN = 0;
    }
    else
    {
		RST_H();			//将复位引脚置高
		//HT7038_RST_PIN = 1;
    }
}


/*************************************
	* 功能函数：	复位ht7038模块
	* 入口参数：	无
	* 返回值：		无
**************************************/
static void HT7038Reset(void)
{
    HT7038ResetSet( ENABLE );
	delay_ms(2); 				//等待100us  
	
    HT7038ResetSet( DISABLE );
	delay_ms(2); 				//等待100us 	
}


/********************************************
	* 功能函数：	ht7038模块初始化
	* 入口参数：	无
	* 返回值：		无
*********************************************/
void HT7038_Init(void)
{
	HT7038_IO_Init();		//复位/片选引脚初始化	
	SPIx_Init();			//SPI1接口初始化
	HT7038Reset();			//硬件复位	
}


/***************************************************************
	* 功能函数：	写特殊命令字操作
	* 入口参数：	command		--	特殊命令字(0xc6/0xc9/0xd3)
					buffer		--	写入的数据
	* 返回值：		无
	* 备注：		详细操作阅读技术手册P24页
****************************************************************/
void HT7038WriteSpecialCommand( uint8_t command, uint8_t *buffer)
{
    uint8_t i;

	CS_L();								//片选拉低(数据传输开始)
	
    SPIx_InOut( command );				//发送特殊写命令	
	
    for( i=0; i<PACK_SIZE; i++ )	//SPI发送3Byte数据
    {
		SPIx_InOut( buffer[i] );	
    }

	CS_H();								//片选拉高(数据传输结束)
}


/***********************************************************************
	* 功能函数：	读取计量/校表参数寄存器的数据(24bit数据)
	* 入口参数：	command	--	命令字(读/写位+地址位)
					buffer	--	数据缓存区
	* 返回值：		无
************************************************************************/
void HT7038MeterORCorrectRegReadData( uint8_t command, uint8_t *buffer)
{
    uint8_t i;

	CS_L();								//片选拉低(数据传输开始)
	
    SPIx_InOut( command & 0x7F );		//发送读命令(0 + 寄存器地址)			
	
	delay_us(2);						//等待2us
    for( i=0; i<PACK_SIZE; i++ )
    {
        buffer[i] = SPIx_InOut( 0 );	//SPI读取一个Byte数据
    }

	CS_H();								//片选拉高(数据传输结束)
}


/**********************************************************
	* 功能函数：	写入校表参数寄存器的数据(16bit数据)
	* 入口参数：	command	--	命令字(读/写位+地址位)
					buffer	--	数据缓存区
	* 返回值：		无
***********************************************************/
void HT7038CorrectRegWriteData( uint8_t command, uint8_t *buffer)
{
    uint8_t i;

	 CS_L();								//片选拉低(数据传输开始)
	
    SPIx_InOut( command | 0x80 );		//发送写命令(1+6bit寄存器地址)			
	
    for( i=0; i<PACK_SIZE; i++ )//SPI发送3Byte数据
    {
		SPIx_InOut( buffer[i] );	
    }

	CS_H();								//片选拉高(数据传输结束)
}


/********************************************************
	* 功能函数：	设置ht7038模块的功能
	* 入口参数：	无
	* 返回值：		无
	* 备注：		主要是写入特殊命令，开启/关闭
					采样通道、滤波器通道
*********************************************************/
void SetHT7038Function(void)
{	
	HT7038WriteSpecialCommand(0xc9,EnableCorrectRegBuf);			//使能SPI校表数据寄存器写操作

	HT7038CorrectRegWriteData(W_MODECFG,ModeConfigBuf);				//开启全部电流/电压ADC通道
	HT7038CorrectRegWriteData(W_MODULECFG,AnalogModuleConfigBuf);	//关闭电压&&电流通道数字高通滤波器

	HT7038WriteSpecialCommand(0xc9,EnableMeterRegBuf);				//关闭SPI校表寄存器的写操作
	HT7038WriteSpecialCommand(0xc6,EnableMeterRegBuf);				//使能SPI计量寄存器读操作
	//HT7038WriteSpecialCommand(0xc6,EnableCorrectRegBuf);			//使能SPI校表寄存器读操作

}



