#include "ht7038.h"


uint8_t	APhaseReceBuf[PACK_SIZE] = {0};							//A�����ݲɼ�����
uint8_t	BPhaseReceBuf[PACK_SIZE] = {0};							//B�����ݲɼ�����

uint8_t 	EnableCorrectRegBuf[PACK_SIZE]   = {0x00,0x00,0x5a};	//ѡ��У��Ĵ���
uint8_t 	EnableMeterRegBuf[PACK_SIZE]     = {0x00,0x00,0x5b};	//ѡ������Ĵ���

//У��Ĵ���������ֵ(����Ĵ������ֳ�Ϊ2Byte����Ҫ���õ�ֵд�ڵ����ֽ���)
uint8_t 	ModeConfigBuf[PACK_SIZE] 		 = {0x00,0x89,0xfe};	//���õ�ѹ/����ADCͨ��
uint8_t 	AnalogModuleConfigBuf[PACK_SIZE] = {0x00,0x35,0x21};	//���ø�ͨ�˲�(�رյ�ѹ&&����ͨ�����ָ�ͨ�˲���)

//static void SetHT7038Function(void);


static void HT7038_IO_Init(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|
							RCC_APB2Periph_GPIOB, ENABLE );
	//Ƭѡ����
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;			//HT7038ģ��Ƭѡ����
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		//ʱ��50MHz
    GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;		//�������
    GPIO_Init( SPI_CS_PORT, &GPIO_InitStructure );			//����ָ��������ʼ��
    GPIO_SetBits(SPI_CS_PORT,GPIO_Pin_4);
	
	//��λ����
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;			//HT7038ģ�鸴λ����
    GPIO_Init( SPI_RST_PORT, &GPIO_InitStructure );			//����ָ��������ʼ��
}


/************************************
	*����������	����RST����״̬
	*��ڲ�����	״ֵ̬(0|1)
	*����ֵ��	��
*************************************/
static void HT7038ResetSet(uint8_t state)
{
    if( state == ENABLE )
    {
		RST_L();			//����λ�����õ�
		//HT7038_RST_PIN = 0;
    }
    else
    {
		RST_H();			//����λ�����ø�
		//HT7038_RST_PIN = 1;
    }
}


/*************************************
	* ���ܺ�����	��λht7038ģ��
	* ��ڲ�����	��
	* ����ֵ��		��
**************************************/
static void HT7038Reset(void)
{
    HT7038ResetSet( ENABLE );
	delay_ms(2); 				//�ȴ�100us  
	
    HT7038ResetSet( DISABLE );
	delay_ms(2); 				//�ȴ�100us 	
}


/********************************************
	* ���ܺ�����	ht7038ģ���ʼ��
	* ��ڲ�����	��
	* ����ֵ��		��
*********************************************/
void HT7038_Init(void)
{
	HT7038_IO_Init();		//��λ/Ƭѡ���ų�ʼ��	
	SPIx_Init();			//SPI1�ӿڳ�ʼ��
	HT7038Reset();			//Ӳ����λ	
}


/***************************************************************
	* ���ܺ�����	д���������ֲ���
	* ��ڲ�����	command		--	����������(0xc6/0xc9/0xd3)
					buffer		--	д�������
	* ����ֵ��		��
	* ��ע��		��ϸ�����Ķ������ֲ�P24ҳ
****************************************************************/
void HT7038WriteSpecialCommand( uint8_t command, uint8_t *buffer)
{
    uint8_t i;

	CS_L();								//Ƭѡ����(���ݴ��俪ʼ)
	
    SPIx_InOut( command );				//��������д����	
	
    for( i=0; i<PACK_SIZE; i++ )	//SPI����3Byte����
    {
		SPIx_InOut( buffer[i] );	
    }

	CS_H();								//Ƭѡ����(���ݴ������)
}


/***********************************************************************
	* ���ܺ�����	��ȡ����/У������Ĵ���������(24bit����)
	* ��ڲ�����	command	--	������(��/дλ+��ַλ)
					buffer	--	���ݻ�����
	* ����ֵ��		��
************************************************************************/
void HT7038MeterORCorrectRegReadData( uint8_t command, uint8_t *buffer)
{
    uint8_t i;

	CS_L();								//Ƭѡ����(���ݴ��俪ʼ)
	
    SPIx_InOut( command & 0x7F );		//���Ͷ�����(0 + �Ĵ�����ַ)			
	
	delay_us(2);						//�ȴ�2us
    for( i=0; i<PACK_SIZE; i++ )
    {
        buffer[i] = SPIx_InOut( 0 );	//SPI��ȡһ��Byte����
    }

	CS_H();								//Ƭѡ����(���ݴ������)
}


/**********************************************************
	* ���ܺ�����	д��У������Ĵ���������(16bit����)
	* ��ڲ�����	command	--	������(��/дλ+��ַλ)
					buffer	--	���ݻ�����
	* ����ֵ��		��
***********************************************************/
void HT7038CorrectRegWriteData( uint8_t command, uint8_t *buffer)
{
    uint8_t i;

	 CS_L();								//Ƭѡ����(���ݴ��俪ʼ)
	
    SPIx_InOut( command | 0x80 );		//����д����(1+6bit�Ĵ�����ַ)			
	
    for( i=0; i<PACK_SIZE; i++ )//SPI����3Byte����
    {
		SPIx_InOut( buffer[i] );	
    }

	CS_H();								//Ƭѡ����(���ݴ������)
}


/********************************************************
	* ���ܺ�����	����ht7038ģ��Ĺ���
	* ��ڲ�����	��
	* ����ֵ��		��
	* ��ע��		��Ҫ��д�������������/�ر�
					����ͨ�����˲���ͨ��
*********************************************************/
void SetHT7038Function(void)
{	
	HT7038WriteSpecialCommand(0xc9,EnableCorrectRegBuf);			//ʹ��SPIУ�����ݼĴ���д����

	HT7038CorrectRegWriteData(W_MODECFG,ModeConfigBuf);				//����ȫ������/��ѹADCͨ��
	HT7038CorrectRegWriteData(W_MODULECFG,AnalogModuleConfigBuf);	//�رյ�ѹ&&����ͨ�����ָ�ͨ�˲���

	HT7038WriteSpecialCommand(0xc9,EnableMeterRegBuf);				//�ر�SPIУ��Ĵ�����д����
	HT7038WriteSpecialCommand(0xc6,EnableMeterRegBuf);				//ʹ��SPI�����Ĵ���������
	//HT7038WriteSpecialCommand(0xc6,EnableCorrectRegBuf);			//ʹ��SPIУ��Ĵ���������

}



