#ifndef _SERVO_H_
#define _SERVO_H_

/*********************************************  includes  *****************************************/
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "main.h"



/*********************************************  functions  *****************************************/
//HAL_StatusTypeDef Servo_SetAngles(uint8_t tim, uint8_t channal, uint32_t pulses);
//HAL_StatusTypeDef Servo_Platform_Search(void);
HAL_StatusTypeDef Servo_Init(void);

//void Servo_Platform_Scan(void);

//void PID_Init(void);

/*********************************************  defines  *****************************************/



/*********************************************  variables  *****************************************/
//extern double Global_Current_Angles_x;
//extern double Global_Current_Angles_y;
//extern uint8_t Target_Flag;
extern uint16_t servo1_angle_pulse;
//extern enum {
//	Target_Detected = 0x001U,
//	No_Target       = 0x000U
//}Target_Exit;

/*�ṹ��extern������Ҫ���������ͬʱ��ͷ�ļ���*/
struct PID{
  double kp;
  double ki;
  double kd;
  signed char passive_error;
  signed short int measured_value; //����ֵ����Ϊ������Ϊ�Ƕȴ�����ˮƽʱΪ0
  uint16_t set_value;
  signed char error; 
  uint16_t output;
  signed int integral;
  signed int derival;
};
extern struct PID positionx_pid, positiony_pid;

#endif
