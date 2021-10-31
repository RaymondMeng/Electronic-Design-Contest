/**
  ******************************************************************************
  * @file    servo.c
  * @author  MengCheng
  * @version V1.0.0
  * @date    2021.10.28
  * @brief   舵机驱动函数
  ******************************************************************************
  * @attention
  *
  * 舵机接口设计如下：
  * Y轴角：TIM9_CHANNEL1 上舵机
  * X轴角：TIM3_CHANNEL1 下舵机
  * 上舵机调角范围：90~180:1500~2250
  * 下舵机调角范围：0~180:700~2340 
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

#include "servo.h"
#include "tim.h"

uint8_t Servo_Status = 1;        //舵机状态：停止或运动

struct PID positionx_pid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
struct PID positiony_pid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
  * @brief  舵机驱动初始化函数
  * @param  None
  * @retval 函数执行状态
  */
void Servo_Init(void)
{
	//HAL_StatusTypeDef status = (HAL_StatusTypeDef)(Servo_SetAngles(1, 1, 90, 0) || Servo_SetAngles(1, 2, 90, 0)); //之所以后面没被执行，是因为被短路了
	Tim_SetPWM(3, 1, 1500);
  Tim_SetPWM(9, 1, 2250);  
  HAL_Delay(1000);
}

/*PID静态参数设置*/
void PID_Init()
{
  positionx_pid.kp = 0.15;
  positionx_pid.ki = 0.001;
  positionx_pid.kd = 0.001;
  positionx_pid.passive_error = 0; //上一次误差量
  positionx_pid.integral = 0; //上一次积分量
  positionx_pid.set_value = 0x50; //激光偏心x轴0x01
  positionx_pid.output = 1500;
  
  positiony_pid.kp = 0.15;
  positiony_pid.ki = 0;
  positiony_pid.kd = 0;
  positiony_pid.passive_error = 0; //上一次误差量
  positiony_pid.integral = 0; //上一次积分量
  positiony_pid.set_value = 0x3c; //激光偏心y轴0x60
  positiony_pid.output = 1500;
}

