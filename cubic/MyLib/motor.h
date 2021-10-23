#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f4xx_hal.h"

struct PID{
  double kp;
  double ki;
  double kd;
  signed char passive_error;
  signed short int measured_value; 
  signed char error; 
  uint16_t set_value;
  uint16_t output;
  signed int integral;
  signed int derival;
};
extern struct PID angle_pid, speed_pid;




#endif
