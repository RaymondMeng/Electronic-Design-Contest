/**
  ******************************************************************************
  * @file    servo.c
  * @author  MengCheng
  * @version V1.0.0
  * @date    2021.5.
  * @brief   舵机驱动函数
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/*********************************************  includes  *****************************************/
#include "servo.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "math.h"


/*********************************************  defines  *****************************************/
//#define Error_Threshold_BigValue 10                                        //误差较大阈值，需后期调节
//#define Error_Threshold_SmallValue 5                                       //误差较小阈值，需后期调节
//#define rotation_adjust_angle1 Error_Threshold_BigValue * (160 - Real_x)   //计算旋转角
//#define pitch_adjust_angle1 Error_Threshold_BigValue * (112 - Real_y)      //计算旋转角
//#define rotation_adjust_angle2 Error_Threshold_SmallValue * (160 - Real_x) / 100 //计算俯仰角          
//#define pitch_adjust_angle2 Error_Threshold_SmallValue * (112 - Real_y) / 100   //计算俯仰角                         
//#define speed_allocate_k 100

/*********************************************  variables  *****************************************/
 
//uint8_t Target_Flag = No_Target; //串口接收到数据并判断后赋值
//double x, y;                   //用于计算调整角度
uint16_t servo1_angle_pulse;  //具体值由θ决定

struct PID positionx_pid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //一定要初始化，外部引用的时候
struct PID positiony_pid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 


/*********************************************  functions  *****************************************/

/**
  * @brief  舵机驱动初始化函数
  * @param  None
  * @retval 函数执行状态
  */
HAL_StatusTypeDef Servo_Init(void)
{
	HAL_StatusTypeDef status = (HAL_StatusTypeDef)(Tim_SetPWM(1, 1, 1500) || Tim_SetPWM(1, 2, 1500));  
  HAL_Delay(1000);
	return status;
}

/*PID静态参数设置*/
void PID_Init()
{
  //第一个pid，对应于x轴，前两题只需用到第一个PID
  positionx_pid.kp = 0.15;
  positionx_pid.ki = 0.001;
  positionx_pid.kd = 0.001;
  positionx_pid.passive_error = 0; //上一次误差量
  positionx_pid.integral = 0; //上一次积分量
  positionx_pid.set_value = 0; //默认水平，
  positionx_pid.output = 1500;
 
//第二个mpu6050 PID，对应于Y轴，第三题用第二个PID，提高部分需要由x的摆动摆动表达式来反推  
  positiony_pid.kp = 0.15;
  positiony_pid.ki = 0;
  positiony_pid.kd = 0;
  positiony_pid.passive_error = 0; //上一次误差量
  positiony_pid.integral = 0; //上一次积分量
  positiony_pid.set_value = pitch2_pulses; //（默认第三题的设定值：即杆与垂直线之间的角θ算出来的对应角度，即舵机1的期望角）
  positiony_pid.output = 1500;
}
	

