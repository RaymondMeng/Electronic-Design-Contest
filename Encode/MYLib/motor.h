#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f1xx_hal.h"

void PID_Init(void); //PID��ʼ��
void Left_A(uint32_t cnt); //С����ǰ�ֵ���
void Left_B(uint32_t cnt); //С������ֵ���
void Right_A(uint32_t cnt); //С����ǰ�ֵ���
void Right_B(uint32_t cnt); //С���Һ��ֵ���
void fw_back(int32_t dist); //С��ǰ������
void broadwise(int32_t dist); //С���������� 

struct PID{
   double kp;
   double ki;
   double kd;
   signed char passive_error;
   signed short int measured_value;
   uint16_t set_value;
   int8_t error;
   uint16_t output;
   int integral;
   int derival;
};
extern struct PID speed_pid, position_pid;


#endif

