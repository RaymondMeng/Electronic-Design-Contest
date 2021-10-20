# STM32_HAL_IIC_SSD1306

#### 介绍
基于HAL库的SSD1306驱动

#### 特性
- 绘制图象时不受 page 限制，可以于屏幕上的任何坐标绘制。
- 图像布尔运算功能，实现图像的加减。
- 数据暂存缓冲区，统一进行通信，实现局部刷新。
- 计算通信较优方案，使得I2C通信次数较少。

#### 安装教程

1.  配置硬件 I2C
2.  拷贝所有代码至项目下
3.  ``include "ssd1306.h"``
4.  修改 ssd1306.h 中配置

#### 配置说明

1.  修改 ssd1306.h 中第11、12行，注释不使用的模式
   ```
    #define OLED_USING_HARDWARE_I2C  //使用硬件i2c
    #define OLED_USING_SOFTWARE_I2C //使用软件i2c
   ```
2.  选择硬件I2C时需要修改 17、19 行中对应的MCU型号
   ```
    #include "stm32f4xx_hal.h"  //修改为所用型号
    ......
    #include "stm32f4xx_hal_i2c.h"
   ```
3.  需要反色显示时打开第22行注释
   ```
   //#define OLED_INVERSE_COLOR  // OLED反色显示
   ```

#### 使用说明

1. 完成屏幕初始化
   ```
   void OLED_Init(I2C_HandleTypeDef);
   ```
2. 调用绘制函数以在缓冲区中绘制图像
    ```
    HAL_StatusTypeDef OLED_BOOL_DrawColumn(uint8_t, uint8_t,  uint8_t*, uint8_t, uint8_t); // 以布尔运算方式绘制不定长列
    HAL_StatusTypeDef OLED_BOOL_DrawChar(uint8_t, uint8_t, uint8_t, uint8_t); // 以布尔运算方式绘制字符
    HAL_StatusTypeDef OLED_BOOL_DrawStr(uint8_t, uint8_t, uint8_t*, uint8_t); // 以布尔运算方式绘制字符串
    HAL_StatusTypeDef OLED_BOOL_DrawBMP(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, uint8_t); // 以布尔运算方式绘制位图
    HAL_StatusTypeDef OLED_DrawColumn(uint8_t, uint8_t, uint8_t*, uint8_t); // 直接绘制不定长列
    HAL_StatusTypeDef OLED_DrawChar(uint8_t, uint8_t, uint8_t); // 直接绘制字符
    HAL_StatusTypeDef OLED_DrawStr(uint8_t, uint8_t, uint8_t*); // 直接绘制字符串
    HAL_StatusTypeDef OLED_DrawBMP(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*); // 直接绘制位图

    ```
3. 刷新缓冲区``void OLED_Refresh(void);``

#### 样例

```
   OLED_Init(hi2c1); // 初始化屏幕
   OLED_BOOL_DrawBMP(0,0, 128, 64, bmp, OLED_BOOL_ADD); // 绘制位图
   OLED_Refresh(); // 刷新缓冲区
```

#### 注意事项

- 绘制位图时以从左至右逐列取模，每8个像素组成一位，保证靠上的像素位于低位，不足8像素列应补充至8位。
- 绘制中文时请以位图方式绘制。