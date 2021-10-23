#include "motor.h"

struct PID angle_pid = {0}, speed_pid = {0}; //结构体初始化


void PID_Init()
{
  angle_pid.kp = 0;
  angle_pid.ki = 0;
  angle_pid.kd = 0;
  angle_pid.passive_error = 0; //上一次的误差
  angle_pid.integral = 0; //上一次的积分量
  angle_pid.set_value = 0; //可以偏心，消除静差
  angle_pid.output = 1200;
  
  speed_pid.kp = 0;
  speed_pid.ki = 0;
  speed_pid.kd = 0;
  speed_pid.passive_error = 0; //上一次的误差
  speed_pid.integral = 0; //上一次的积分量
  speed_pid.set_value = 0; //可以偏心，消除静差
  speed_pid.output = 1200;
}

