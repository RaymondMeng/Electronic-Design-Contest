
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
#define ADDRESS_W 0xD0   //д��ַ 0x68
#define ADDRESS_R 0xD1   //����ַ  0x69
#define MPU_PWR_MGMT1_REG		0X6B	//��Դ����Ĵ���1
#define MPU_GYRO_CFG_REG		0X1B	//���������üĴ���
#define MPU_ACCEL_CFG_REG		0X1C	//���ٶȼ����üĴ���
#define MPU_SAMPLE_RATE_REG		0X19	//�����ǲ���Ƶ�ʷ�Ƶ��
#define MPU_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���
#define MPU_USER_CTRL_REG		0X6A	//�û����ƼĴ���
#define MPU_FIFO_EN_REG			0X23	//FIFOʹ�ܼĴ���
#define MPU_INTBP_CFG_REG		0X37	//�ж�/��·���üĴ���
#define MPU_DEVICE_ID_REG		0X75	//����ID�Ĵ���
#define MPU_PWR_MGMT2_REG		0X6C	//��Դ����Ĵ���2 
#define MPU_CFG_REG				0X1A	//���üĴ��� ��ͨ�˲������üĴ���
#define MPU_TEMP_OUTH_REG		0X41	//�¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG		0X42	//�¶�ֵ��8λ�Ĵ���

#define MPU_ACCEL_XOUTH_REG		0X3B	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG		0X3C	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG		0X3D	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG		0X3E	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG		0X3F	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG		0X40	//���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG		0X43	//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG		0X44	//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG		0X45	//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG		0X46	//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG		0X47	//������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG		0X48	//������ֵ,Z���8λ�Ĵ���

//��������ٶ�
#define DEFAULT_MPU_HZ  (100)		//100Hz

//q30��ʽ,longתfloatʱ�ĳ���.
#define q30  1073741824.0f
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
void MPU_6050_Init(void);
unsigned char  MPU_Set_LPF(unsigned short lpf);
//DMP����Ҫ�ĺ���
unsigned char HAL_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
unsigned char HAL_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
//��ȡ �¶ȴ�����   ������  ���ٶ�����
void read_all(void);
//�����Ƿ������
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
//����ת��
unsigned short inv_row_2_scale(const signed char *row);
//MPU6050�Բ���
//����ֵ:0,����
//    ����,ʧ��
unsigned char run_self_test(void);
//mpu6050,dmp��ʼ��
//����ֵ:0,����
//    ����,ʧ��
unsigned char mpu_dmp_init(void);
//�õ�dmp����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//�����Ƿ�������
static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};

float pitch,roll,yaw; 		//ŷ����
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
	MPU_6050_Init();// ���Գ��� ֱ�Ӵ�FIFO
	//��ʼ��DMP
	DMP_INT_FLAG=mpu_dmp_init();
	printf("DMP_INT_FLAG %d \n",DMP_INT_FLAG);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//read_all();//��ȡ ������ ���ٶ� �¶� ԭʼ����
//		rev_flag=mpu_dmp_get_data(&pitch,&roll,&yaw);
//	  printf("rev_flag %d\n",rev_flag);
		if((rev_flag=mpu_dmp_get_data(&pitch,&roll,&yaw))==0)
		{
			printf("%.2f %.2f %.2f \n",pitch,roll,yaw);
		}
		else 
		{
				printf("rev_flag %d\n",rev_flag);
				HAL_Delay(100);//��ȡƵ�ʲ���̫�� ��ֹFIFO���
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
	//����豸�Ƿ�׼����  ��ַ   ��� ���� ��ʱʱ��  
	status=HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_W, 10, HAL_MAX_DELAY);
	printf("status is %d\n",status);
	//��������Ƿ�׼����
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//����д�Ĵ�������
	pdata=0x80; //��λMPU
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); //д0x80��λ
	status=HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_W, 10, HAL_MAX_DELAY);
	printf("status is %d\n",status);
	
	HAL_Delay(500);  //��λ����Ҫ�ȴ�һ��ʱ�� �ȴ�оƬ��λ��� 
	
	//��������Ƿ�׼����
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//����MPU
	pdata=0x01; // 7λ 1 ����  6λ˯��ģʽ1 ˯�� 2 ����   3λ Ϊu�㶮������0 ����    0-2λ ʱ��ѡ��  01 PLLʹ��XZ��������Ϊ����
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); //д0x80��λ
	//�������� �Ĵ���  2000  3
	pdata=3<<3; //����3 Ϊ����2000  ����3λ ����Ӧ�� 3 4 λ�Ĵ�����
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_GYRO_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//���ü��ٶȴ��������� 2g
	pdata=0; 
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_ACCEL_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//�����ǲ�����Ƶ����
	pdata=19; //1000/50-1  �������Ҫ�����ϲ鿴  ԭ�� �ͼ��㷽��
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_SAMPLE_RATE_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	//�ر������ж�
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_INT_EN_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//I2C ��ģʽ�� ��Ӵ����������ӿڹر�
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_USER_CTRL_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//�ر�FIFO 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_FIFO_EN_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//�ж���·���� �͵�ƽ��Ч
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0X80;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_INTBP_CFG_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	//���õ�ͨ�˲���
	MPU_Set_LPF(50/2);
	//������ID  Ĭ���� 0x68 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=MPU_DEVICE_ID_REG;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_DEVICE_ID_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("ID is %X \n",pdata);
	//ʹ�� ������ �ͼ��ٶ� ����
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT2_REG, 1, &pdata, 1, HAL_MAX_DELAY);
//�Ĵ���������Թۿ�
/*
	//��ȡ�Ĵ���1 
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	pdata=0;
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_PWR_MGMT1_REG, 1, &pdata, 1, HAL_MAX_DELAY); 
	printf("0x6B REG1 %02X \n",pdata);
	//��ȡ�¶ȴ�����
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
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
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
		//MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

//��ȡ �¶ȴ�����   ������  ���ٶ�ԭʼ����
void read_all(void)
{
	HAL_StatusTypeDef status;
	HAL_I2C_StateTypeDef flag;
	unsigned char tem[2]={0};
	unsigned char  accbuff[6]={0};  //MPU_ACCEL_XOUTH_REG ���ٶ� 
	unsigned char tuoluo[6]={0};
	short aacx,aacy,aacz; //���ٶ�
	short tolx,toly,tolz;
	 short raw;
	float temp;
	short  lingmin=16384;//������ 16384 
	float tuol=16.4f;
	flag=HAL_I2C_GetState(&hi2c1);
	printf("flag %X \n",flag);
	status=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_W, MPU_TEMP_OUTH_REG, 1, tem, 2, HAL_MAX_DELAY); 
	printf("status %02X \n",status);
	raw=((unsigned short)tem[0]<<8)|tem[1];  
	printf("%d %d \n",tem[0],tem[1]);
	temp=36.53+((double)raw)/340;  
	printf("%3.3f C\n",temp);
	//��ȡ���ٶ�3��ߵ�λ����
	status=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_W, MPU_ACCEL_XOUTH_REG, 1, accbuff, 6, HAL_MAX_DELAY); 
	aacx=((short)(accbuff[0]<<8))|accbuff[1];
	aacy=((short)(accbuff[2]<<8))|accbuff[3];
	aacz=((short)(accbuff[4]<<8))|accbuff[5];
	
	printf("acc is  x %d y %d z %d   x%.1f g,y%.1f g,z%.1f g\n",aacx,aacy,aacz,
																																	(float)(aacx)/lingmin,
																																	(float)(aacy)/lingmin,
																																	(float)(aacz)/lingmin																																		
																																	);
	//��ȡ������3��  MPU_GYRO_XOUTH_REG
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
//�����ں�DMP��ĺ���  
unsigned char HAL_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	printf("flag %X \n",HAL_I2C_GetState(&hi2c1));
	//DMP������  slave_addr ��ַ��0x68 ����ʱ��Ҫ����1λ���һλ���϶�дλ
	HAL_I2C_Mem_Write(&hi2c1, ((slave_addr<<1)|0), reg_addr, 1, (unsigned char *)data, length, HAL_MAX_DELAY);
	return 0;
}
unsigned char HAL_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	printf("flag %X \n",HAL_I2C_GetState(&hi2c1));
	//DMP������  slave_addr ��ַ��0x68 ����ʱ��Ҫ����1λ���һλ���϶�дλ
	HAL_I2C_Mem_Read(&hi2c1, ((slave_addr<<1)|1), reg_addr, 1, (unsigned char *)data, length, HAL_MAX_DELAY);
	return 0;
}
//mpu6050,dmp��ʼ��
//����ֵ:0,����
//    ����,ʧ��
unsigned char mpu_dmp_init(void)
{
	unsigned char res=0;
	//IIC_Init(); 		//��ʼ��IIC����
	if(mpu_init()==0)	//��ʼ��MPU6050
	{	 
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//��������Ҫ�Ĵ�����
		if(res)return 1; 
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//����FIFO
		if(res)return 2; 
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//���ò�����
		if(res)return 3; 
		res=dmp_load_motion_driver_firmware();		//����dmp�̼�
		if(res)return 4; 
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//���������Ƿ���
		if(res)return 5; 
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//����dmp����
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res)return 6; 
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//����DMP�������(��󲻳���200Hz)
		if(res)return 7;   
		res=run_self_test();		//�Լ�
		if(res)return 8;    
		res=mpu_set_dmp_state(1);	//ʹ��DMP
		if(res)return 9;     
	}
	return 0;
}
//�����Ƿ������
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
//����ת��
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
//MPU6050�Բ���
//����ֵ:0,����
//    ����,ʧ��
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
//�õ�dmp����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
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
		q0 = quat[0] / q30;	//q30��ʽת��Ϊ������
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//����õ�������/�����/�����
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
