#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f1xx_hal.h"

void PID_Init(void); //PID初始化
void Left_A(uint32_t cnt); //小车左前轮调速
void Left_B(uint32_t cnt); //小车左后轮调速
void Right_A(uint32_t cnt); //小车右前轮调速
void Right_B(uint32_t cnt); //小车右后轮调速
void fw_back(int32_t dist); //小车前进后退
void broadwise(int32_t dist); //小车横向行走 

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

