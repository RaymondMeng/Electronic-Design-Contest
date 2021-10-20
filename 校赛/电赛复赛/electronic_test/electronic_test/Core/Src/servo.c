/**
  ******************************************************************************
  * @file    servo.c
  * @author  MengCheng
  * @version V1.0.0
  * @date    2021.5.
  * @brief   �����������
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/*********************************************  includes  *****************************************/
#include "servo.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "math.h"


/*********************************************  defines  *****************************************/
//#define Error_Threshold_BigValue 10                                        //���ϴ���ֵ������ڵ���
//#define Error_Threshold_SmallValue 5                                       //����С��ֵ������ڵ���
//#define rotation_adjust_angle1 Error_Threshold_BigValue * (160 - Real_x)   //������ת��
//#define pitch_adjust_angle1 Error_Threshold_BigValue * (112 - Real_y)      //������ת��
//#define rotation_adjust_angle2 Error_Threshold_SmallValue * (160 - Real_x) / 100 //���㸩����          
//#define pitch_adjust_angle2 Error_Threshold_SmallValue * (112 - Real_y) / 100   //���㸩����                         
//#define speed_allocate_k 100

/*********************************************  variables  *****************************************/
 
//uint8_t Target_Flag = No_Target; //���ڽ��յ����ݲ��жϺ�ֵ
//double x, y;                   //���ڼ�������Ƕ�
uint16_t servo1_angle_pulse;  //����ֵ�ɦȾ���

struct PID positionx_pid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //һ��Ҫ��ʼ�����ⲿ���õ�ʱ��
struct PID positiony_pid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 


/*********************************************  functions  *****************************************/

/**
  * @brief  ���������ʼ������
  * @param  None
  * @retval ����ִ��״̬
  */
HAL_StatusTypeDef Servo_Init(void)
{
	HAL_StatusTypeDef status = (HAL_StatusTypeDef)(Tim_SetPWM(1, 1, 1500) || Tim_SetPWM(1, 2, 1500));  
  HAL_Delay(1000);
	return status;
}

/*PID��̬��������*/
void PID_Init()
{
  //��һ��pid����Ӧ��x�ᣬǰ����ֻ���õ���һ��PID
  positionx_pid.kp = 0.15;
  positionx_pid.ki = 0.001;
  positionx_pid.kd = 0.001;
  positionx_pid.passive_error = 0; //��һ�������
  positionx_pid.integral = 0; //��һ�λ�����
  positionx_pid.set_value = 0; //Ĭ��ˮƽ��
  positionx_pid.output = 1500;
 
//�ڶ���mpu6050 PID����Ӧ��Y�ᣬ�������õڶ���PID����߲�����Ҫ��x�İڶ��ڶ����ʽ������  
  positiony_pid.kp = 0.15;
  positiony_pid.ki = 0;
  positiony_pid.kd = 0;
  positiony_pid.passive_error = 0; //��һ�������
  positiony_pid.integral = 0; //��һ�λ�����
  positiony_pid.set_value = pitch2_pulses; //��Ĭ�ϵ�������趨ֵ�������봹ֱ��֮��ĽǦ�������Ķ�Ӧ�Ƕȣ������1�������ǣ�
  positiony_pid.output = 1500;
}
	

