/*
 * control.h
 *
 *  Created on: Oct 9, 2020
 *      Author: 10970
 */

#ifndef STATE_SPACE_CONTROL_H_
#define STATE_SPACE_CONTROL_H_

#endif

/* STATE_SPACE_CONTROL_H_ */
struct _pid
{
	float ExpectedValue;//定义设定值
	float ActualValue;//定义实际值
	float err;//定义偏差值
	float err_last;//定义上一个偏差值
	float err_prev;//定义前一个的偏差值
	float Kp, Ki, Kd;//定义比例、积分、微分系数
}pid_pitch,pid_yaw;

void PID_Init();
void setFrenquency(float acc);
float accel(float angle,struct _pid *pid);
