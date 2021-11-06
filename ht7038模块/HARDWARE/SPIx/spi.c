#include "spi.h"


void SPIx_Init( void )
{
    SPI_InitTypeDef		SPI_InitStructure;
    GPIO_InitTypeDef 	GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd( SPI_PIN_SCK_PORT_CLK  |
							SPI_PIN_MISO_PORT_CLK | 			//ʹ��GPIOxʱ��
							SPI_PIN_MOSI_PORT_CLK , ENABLE );
    RCC_APB2PeriphClockCmd( SPI_CLK, ENABLE );					//ʹ��SPI1ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );		//ʹ�ܸ�������ʱ��
	
    //GPIO configuration 
	GPIO_InitStructure.GPIO_Pin 	= SPI_PIN_SCK;				//ʱ��CLK����
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_AF_PP;
    GPIO_Init( SPI_PIN_SCK_PORT, &GPIO_InitStructure );			//����ָ��������ʼ��
	
    GPIO_InitStructure.GPIO_Pin 	= SPI_PIN_MOSI;				//MOSI����
    GPIO_Init( SPI_PIN_MOSI_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;  	//MISO����
    GPIO_InitStructure.GPIO_Pin 	= SPI_PIN_MISO;
    GPIO_Init( SPI_PIN_MISO_PORT, &GPIO_InitStructure );

    //SPI_INTERFACE Config 
    SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;	//ȫ��ȫ˫��
    SPI_InitStructure.SPI_Mode 				= SPI_Mode_Master;					//����ģʽ
    SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;					//���ݿ��Ϊ8bit
    SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_Low;						//ʱ�ӿ���״̬ΪL��ƽ
    SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_2Edge;					//��2�������ؿ�ʼ����(�½��ز���)
    SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;						//�������NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; 			//ʱ��ʱ��9MHz
    SPI_InitStructure.SPI_FirstBit			= SPI_FirstBit_MSB;					//�����λ��ʼ����
    SPI_InitStructure.SPI_CRCPolynomial 	= 7;								//CRC����ʽϵ��Ϊ7
    SPI_Init( SPI_INTERFACE, &SPI_InitStructure );								//��ʼ��SPI
	
    SPI_Cmd( SPI_INTERFACE, ENABLE );	//ʹ��SPI1
}


/**************************************************
	* ���ܺ�����	SPIд��/��ȡһ��Byte����
	* ��ڲ�����	outData	--	Ҫ���͵�����
	* ����ֵ��		��ȡ������
***************************************************/
u8 SPIx_InOut( u8 outData )
{
    //��������
    SPI_I2S_SendData( SPI_INTERFACE, outData );
	
	//�ȴ����ձ�־λΪ"1"
    while( SPI_I2S_GetFlagStatus( SPI_INTERFACE, SPI_I2S_FLAG_RXNE ) == RESET );
	
	//���ؽ��յ�������
    return SPI_I2S_ReceiveData( SPI_INTERFACE );
}



