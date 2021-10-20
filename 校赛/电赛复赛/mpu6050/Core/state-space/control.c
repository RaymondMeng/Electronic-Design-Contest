/*
 * control.c
 *
 *  Created on: Oct 9, 2020
 *      Author: 10970
 */

#include "control.h"
#include "tim.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "stm32_hal_legacy.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "stdio.h"

#define USE_HAL_LEGACY

const uint8_t K = 100;



void PID_Init()
{
	pid_pitch.ExpectedValue = 0.0;
	pid_pitch.ActualValue = 0.0;
	pid_pitch.err = 0.0;
	pid_pitch.err_prev = 0.0;
	pid_pitch.err_last = 0.0;
	pid_pitch.Kp = 10;
	pid_pitch.Ki = 0.0;
	pid_pitch.Kd = 0;

	pid_yaw.ExpectedValue = 0.0;
	pid_yaw.ActualValue = 0.0;
	pid_yaw.err = 0.0;
	pid_yaw.err_prev = 0.0;
	pid_yaw.err_last = 0.0;
	pid_yaw.Kp = 0.0;
	pid_yaw.Ki = 0.0;
	pid_yaw.Kd = 0.0;


}



float accel(float angle,struct _pid *pid)
{
	float delta_omega;

	pid->err = pid->ExpectedValue - angle;

	delta_omega =
			pid->Kp * pid->err +
			pid->Kd*(pid->err - pid->err_prev);

	pid->err_prev = pid->err_last;
	pid->err_last = pid->err;
	pid->ExpectedValue = pid->ExpectedValue - pid->Ki * ( + pid->err_prev + pid->err_last);

	return delta_omega;
}

void setFrenquency(float acc)
{
	//频率500hz-64khz pulse/rev=800 即 0.625r/s ---- 80r/s 3.9269909rad/s ---502.6548384rad/s

	extern float velocity;
	velocity += acc * 0.005;//200hz
	if(velocity<0){

		HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, RESET);
		if(velocity<-90)velocity=-90;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,10-velocity);
	}
	else{

		HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, SET);
		if(velocity>90)velocity=90;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,10+velocity);
	}








}


