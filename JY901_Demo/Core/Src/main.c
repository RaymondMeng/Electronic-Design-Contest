/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * 2021年10月7日调试完毕，串口1接收九轴传感器的数据，串口2调试输出数据
  * 可输出角度信息以及时间，加速度以及速度信息
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t temp;
uint8_t RxBuffer[10] = "\0";
float Roll, Pitch, Yaw, Tempera;
float ax, ay, az;
float sx, sy, sz;
/**
  * @brief  串口调试
  * @retval None
  */
#include "stdio.h"
#define DEBUG
#ifdef DEBUG
#define DBG(format, ...) fprintf(stdout, "[\tDBG](File:%s, Func:%s(), Line:%d): " \
                                , __FILE__, __FUNCTION__, __LINE__);             \
                         fprintf(stdout, format"\r\n", ##__VA_ARGS__)
#else
#define DBG(format, ...)  do {} while (0)
#endif

int fputc(int ch, FILE *f){
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart2, temp, 1, 2);//huart1 根据配置修改
    return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, &temp, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    printf("翻滚角：%d\r\n", (int)Roll);
    printf("俯仰角：%d\r\n", (int)Pitch);
    printf("偏航角：%d\r\n\r\n", (int)Yaw);
    
    printf("角加速度：%d %d %d\r\n", (int)ax, (int)ay, (int)az);
    
    printf("角速度：%d %d %d\r\n", (int)sx, (int)sy, (int)sz);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  static uint8_t count = 0;
  uint16_t sum;
  static uint8_t mode;
  //printf("helloworld!");
  if(huart -> Instance == huart1.Instance ) {
    if(temp == 0x55 && count == 0)
      count = 1;
    else if (count == 1 && temp >= 0x51 && temp <= 0x53)
    {
      count = 2;
      mode = temp;
    }
    else if (count == 2)
    {
       RxBuffer[0] = temp;
       count = 3;
       
    }
    else if (count == 3)
    {
       RxBuffer[1] = temp;
       count = 4;
    }
    else if (count == 4)
    {
       RxBuffer[2] = temp;
       count = 5;
    }
    else if (count == 5)
    {
       RxBuffer[3] = temp;
       count = 6;
    }
    else if (count == 6)
    {
       RxBuffer[4] = temp;
       count = 7;
    }
    else if (count == 7)
    {
       RxBuffer[5] = temp;
       count = 8;
    }
    else if (count == 8)
    {
       RxBuffer[6] = temp;
       count = 9;
    }
    else if (count == 9)
    {
       RxBuffer[7] = temp;
       count = 10; 
//       sum = 0x55 + 0x53 + RxBuffer[0] + RxBuffer[1] + RxBuffer[2] + RxBuffer[3] + RxBuffer[4] + \
//             RxBuffer[5] + RxBuffer[6] + RxBuffer[7];
      //printf("helloworld!");
    }
    else if (count == 10)
    {
       switch (mode)
       {
          case 0x51: 
                ax = (float)((unsigned short)(RxBuffer[1]<<8)|RxBuffer[0]) / 32768 * 16;
                ay = (float)((unsigned short)(RxBuffer[3]<<8)|RxBuffer[2]) / 32768 * 16;
                az = (float)((unsigned short)(RxBuffer[5]<<8)|RxBuffer[4]) / 32768 * 16;
                break;
          case 0x52:
                sx = (float)((unsigned short)(RxBuffer[1]<<8)|RxBuffer[0]) / 32768 * 2000;
                sy = (float)((unsigned short)(RxBuffer[3]<<8)|RxBuffer[2]) / 32768 * 2000;
                sz = (float)((unsigned short)(RxBuffer[5]<<8)|RxBuffer[4]) / 32768 * 2000;
                break;
          case 0x53: 
                Roll    = (float)((unsigned short)(RxBuffer[1]<<8)|RxBuffer[0]) / 32768 * 180;
                Pitch   = (float)((unsigned short)(RxBuffer[3]<<8)|RxBuffer[2]) / 32768 * 180;
                Yaw     = (float)((unsigned short)(RxBuffer[5]<<8)|RxBuffer[4]) / 32768 * 180;
                Tempera = (float)((unsigned short)(RxBuffer[7]<<8)|RxBuffer[6]) / 100;
                break;
       }
       
       //printf("helloworld!");
       count = 0;
    }
    
    else
       count = 0;
  }
  HAL_UART_Receive_IT(&huart1, &temp, 1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
