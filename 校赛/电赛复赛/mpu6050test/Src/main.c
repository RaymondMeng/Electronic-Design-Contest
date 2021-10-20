
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include <math.h>
#define ADDRESS_W 0xD0   //写地址 0x68
#define ADDRESS_R 0xD1   //读地址  0x69
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_SAMPLE_RATE_REG		0X19	//陀螺仪采样频率分频器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_CFG_REG				0X1A	//配置寄存器 低通滤波器配置寄存器
#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

//定义输出速度
#define DEFAULT_MPU_HZ  (100)		//100Hz

//q30格式,long转float时的除数.
#define q30  1073741824.0f
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
void MPU_6050_Init(void);
unsigned char  MPU_Set_LPF(unsigned short lpf);
//DMP库需要的函数
unsigned char HAL_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
unsigned char HAL_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
//读取 温度传感器   陀螺仪  加速度数据
void read_all(void);
//陀螺仪方向控制
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
//方向转换
unsigned short inv_row_2_scale(const signed char *row);
//MPU6050自测试
//返回值:0,正常
//    其他,失败
unsigned char run_self_test(void);
//mpu6050,dmp初始化
//返回值:0,正常
//    其他,失败
unsigned char mpu_dmp_init(void);
//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
//pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
//roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
//yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
//返回值:0,正常
//    其他,失败
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//陀螺仪方向设置
static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};

float pitch,roll,yaw; 		//欧拉角
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	unsigned char DMP_INT_FLAG=0;
	unsigned char rev_flag=0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	printf("MPU6050test\n");
	MPU_6050_Init();// 可以尝试 直接打开FIFO
	//初始化DMP
	DMP_INT_FLAG=mpu_dmp_init();
	printf("DMP_INT_FLAG %d \n",DMP_INT_FLAG);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//read_all();//读取 陀螺仪 加速度 温度 原始数据
//		rev_flag=mpu_dmp_get_data(&pitch,&roll,&yaw);
//	  printf("rev_flag %d\n",rev_flag);
		if((rev_flag=mpu_dmp_get_data(&pitch,&roll,&yaw))==0)
		{
			printf("%.2f %.2f %.2f \n",pitch,roll,yaw);
		}
		else 
		{
				printf("rev_flag %d\n",rev_flag);
				HAL_Delay(100);//读取频率不能太慢 防止FIFO溢出
		}
	
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enables the Clock Security System 
    */
  HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void MPU_6050_Init(void)
{
	HAL_StatusTypeDef status;
	HAL_I2C_StateTypeDef flag;
	unsigned char pdata;
	//检查设备是否准备好  地址   检查 次数 超时时间  
	status=HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_W, 10, HAL_MAX_DELAY);
	printf("status is %d\n",status);
	//检查总线是否准备好
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//发送写寄存器命令
	pdata=0x80; //复位MPU
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); //写0x80复位
	status=HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_W, 10, HAL_MAX_DELAY);
	printf("status is %d\n",status);
	
	HAL_Delay(500);  //复位后需要等待一段时间 等待芯片复位完成 
	
	//检查总线是否准备好
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//唤醒MPU
	pdata=0x01; // 7位 1 重启  6位睡眠模式1 睡眠 2 唤醒   3位 为u你懂传感器0 开启    0-2位 时钟选择  01 PLL使用XZ轴陀螺作为参数
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); //写0x80复位
	//设陀螺仪 寄存器  2000  3
	pdata=3<<3; //设置3 为量程2000  右移3位 到对应的 3 4 位寄存器中
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_GYRO_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//设置加速度传感器量程 2g
	pdata=0; 
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_ACCEL_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//陀螺仪采样分频设置
	pdata=19; //1000/50-1  这个还需要查资料查看  原因 和计算方法
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_SAMPLE_RATE_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//关闭所有中断
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_INT_EN_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//I2C 主模式的 外接磁力传感器接口关闭
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_USER_CTRL_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//关闭FIFO 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_FIFO_EN_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//中断旁路设置 低电平有效
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0X80;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_INTBP_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//设置低通滤波器
	MPU_Set_LPF(50/2);
	//读器件ID  默认是 0x68 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=MPU_DEVICE_ID_REG;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_DEVICE_ID_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("ID is %X \n",pdata);
	//使能 陀螺仪 和加速度 工作
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT2_REG, 1, &pdata, 1, HAL_MAX_DELAY);
//寄存器输出调试观看
/*
	//读取寄存器1 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("0x6B REG1 %02X \n",pdata);
	//读取温度传感器
	HAL_Delay(500);
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0xff;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_TEMP_OUTH_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("hight bit %02X \n",pdata);
	HAL_Delay(500);
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0xff;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_TEMP_OUTH_REG+1, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("low bit %02X \n",pdata);
*/
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
unsigned char  MPU_Set_LPF(unsigned short lpf)
{
	unsigned char data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	
	return HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_CFG_REG, 1, &data, 1, HAL_MAX_DELAY); 
		//MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}

//读取 温度传感器   陀螺仪  加速度原始数据
void read_all(void)
{
	HAL_StatusTypeDef status;
	HAL_I2C_StateTypeDef flag;
	unsigned char tem[2]={0};
	unsigned char  accbuff[6]={0};  //MPU_ACCEL_XOUTH_REG 加速度 
	unsigned char tuoluo[6]={0};
	short aacx,aacy,aacz; //加速度
	short tolx,toly,tolz;
	 short raw;
	float temp;
	short  lingmin=16384;//灵敏度 16384 
	float tuol=16.4f;
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	status=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_W, MPU_TEMP_OUTH_REG, 1, tem, 2, HAL_MAX_DELAY); 
	printf("status %02X \n",status);
	raw=((unsigned short)tem[0]<<8)|tem[1];  
	printf("%d %d \n",tem[0],tem[1]);
	temp=36.53+((double)raw)/340;  
	printf("%3.3f C\n",temp);
	//读取加速度3轴高低位数据
	status=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_W, MPU_ACCEL_XOUTH_REG, 1, accbuff, 6, HAL_MAX_DELAY); 
	aacx=((short)(accbuff[0]<<8))|accbuff[1];
	aacy=((short)(accbuff[2]<<8))|accbuff[3];
	aacz=((short)(accbuff[4]<<8))|accbuff[5];
	
	printf("acc is  x %d y %d z %d   x%.1f g,y%.1f g,z%.1f g\n",aacx,aacy,aacz,
																																	(float)(aacx)/lingmin,
																																	(float)(aacy)/lingmin,
																																	(float)(aacz)/lingmin																																		
																																	);
	//读取陀螺仪3轴  MPU_GYRO_XOUTH_REG
	status=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_W, MPU_GYRO_XOUTH_REG, 1, tuoluo, 6, HAL_MAX_DELAY); 
	tolx=((unsigned short)(tuoluo[0]<<8))|tuoluo[1];
	toly=((unsigned short)(tuoluo[2]<<8))|tuoluo[3];
	tolz=((unsigned short)(tuoluo[4]<<8))|tuoluo[5];		
	printf("tuoluo is  x %d y %d z %d   x%.1f ,y%.1f ,z%.1f \n",tolx,toly,tolz,
																																	(float)(tolx)/tuol,
																																	(float)(toly)/tuol,
																																	(float)(tolz)/tuol																																		
																																	);																																
} 
//定义融合DMP库的函数  
unsigned char HAL_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	printf("flag %X \n",HAL_I2C_GetState(&hi2c1));
	//DMP库驱动  slave_addr 地址是0x68 发送时需要左移1位最后一位或上读写位
	HAL_I2C_Mem_Write(&hi2c1, ((slave_addr<<1)|0), reg_addr, 1, (unsigned char *)data, length, HAL_MAX_DELAY);
	return 0;
}
unsigned char HAL_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	printf("flag %X \n",HAL_I2C_GetState(&hi2c1));
	//DMP库驱动  slave_addr 地址是0x68 发送时需要左移1位最后一位或上读写位
	HAL_I2C_Mem_Read(&hi2c1, ((slave_addr<<1)|1), reg_addr, 1, (unsigned char *)data, length, HAL_MAX_DELAY);
	return 0;
}
//mpu6050,dmp初始化
//返回值:0,正常
//    其他,失败
unsigned char mpu_dmp_init(void)
{
	unsigned char res=0;
	//IIC_Init(); 		//初始化IIC总线
	if(mpu_init()==0)	//初始化MPU6050
	{	 
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//设置所需要的传感器
		if(res)return 1; 
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置FIFO
		if(res)return 2; 
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//设置采样率
		if(res)return 3; 
		res=dmp_load_motion_driver_firmware();		//加载dmp固件
		if(res)return 4; 
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(res)return 5; 
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//设置dmp功能
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res)return 6; 
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//设置DMP输出速率(最大不超过200Hz)
		if(res)return 7;   
		res=run_self_test();		//自检
		if(res)return 8;    
		res=mpu_set_dmp_state(1);	//使能DMP
		if(res)return 9;     
	}
	return 0;
}
//陀螺仪方向控制
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}
//方向转换
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}
//MPU6050自测试
//返回值:0,正常
//    其他,失败
unsigned char run_self_test(void)
{
	int result;
	//char test_packet[4] = {0};
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		/* Test passed. We can trust the gyro data here, so let's push it down
		* to the DMP.
		*/
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return 0;
	}else return 1;
}
//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
//pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
//roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
//yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
//返回值:0,正常
//    其他,失败
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30格式转换为浮点数
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//计算得到俯仰角/横滚角/航向角
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
