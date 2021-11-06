#include "timer.h"


u16 Timer3_Conuter = 0;
u16	Conuter = 0;


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 			//��ʱ��3ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period 		= arr; 					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler 	= psc; 					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);						//ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel 				 	 = TIM3_IRQn;	//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  			//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 3;  			//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE; 		//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);										//��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);												//ʹ��TIMx					 
}


//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		Timer3_Conuter++;
		Conuter++;
		
		//MeterHT7038ReadData(R_SAMPLE_IA,A_Rece_Data);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  	//���TIMx�����жϱ�־ 
	}
}







