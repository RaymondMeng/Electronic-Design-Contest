#include "led.h"


//LED_WORK I/O�ڳ�ʼ��
void LEDx_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;			//LED_WORK-->PB.2 �˿�����
									  			
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB.2
	GPIO_SetBits(GPIOB,GPIO_Pin_2);							//PB.2�����(�رյ�)
}
 



