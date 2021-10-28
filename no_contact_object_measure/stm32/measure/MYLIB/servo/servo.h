#ifndef _SERVO_H_
#define _SERVO_H_

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "math.h"

void Servo_Init(void);
void Sero_Platform_Track(uint16_t target_x, uint16_t target_y);
void PID_Init(void);

struct PID{
  double kp;
  double ki;
  double kd;
  signed char passive_error;
  signed short int measured_value;
  uint16_t set_value;
  signed char error; 
  uint16_t output;
  signed int integral;
  signed int derival;
};
extern struct PID positionx_pid, positiony_pid;

#endif
