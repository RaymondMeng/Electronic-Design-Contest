#include "motor.h"

struct PID angle_pid = {0}, speed_pid = {0}; //�ṹ���ʼ��


void PID_Init()
{
  angle_pid.kp = 0;
  angle_pid.ki = 0;
  angle_pid.kd = 0;
  angle_pid.passive_error = 0; //��һ�ε����
  angle_pid.integral = 0; //��һ�εĻ�����
  angle_pid.set_value = 0; //����ƫ�ģ���������
  angle_pid.output = 1200;
  
  speed_pid.kp = 0;
  speed_pid.ki = 0;
  speed_pid.kd = 0;
  speed_pid.passive_error = 0; //��һ�ε����
  speed_pid.integral = 0; //��һ�εĻ�����
  speed_pid.set_value = 0; //����ƫ�ģ���������
  speed_pid.output = 1200;
}

