/*
 * @Author: Snitro
 * @Date: 2021-02-23 17:42:27
 * @LastEditors: Snitro
 * @LastEditTime: 2021-02-24 15:30:55
 * @Description: SSD1306驱动
 */
#ifndef __SSD1306_H
#define __SSD1306_H

#define OLED_USING_HARDWARE_I2C  //使用硬件i2c
//#define OLED_USING_SOFTWARE_I2C //使用软件i2c

#include <string.h>

#include "FontLib.h"
#include "stm32f4xx_hal.h"  //修改为所用型号
#ifdef OLED_USING_HARDWARE_I2C
#include "stm32f4xx_hal_i2c.h"
#endif

//#define OLED_INVERSE_COLOR  // OLED反色显示
#define OLED_Addr 0x78  // OLED IIC 地址

#define CmdReg 0x00     //表示发送指令
#define DataReg 0x40    //表示发送数据
#define OLED_ON 0xaf    //打开显示器
#define OLED_OFF 0xae   //关闭显示器
#define OLED_Width 128  // OLED宽度128像素
#define OLED_High 64    // OLED高度64像素

#define OLED_BOOL_Replace ((uint8_t)0x00)  //替换
#define OLED_BOOL_ADD ((uint8_t)0x01)      //加
#define OLED_BOOL_Subtract ((uint8_t)0x02) //减

#ifdef OLED_USING_HARDWARE_I2C
void OLED_Init(I2C_HandleTypeDef);
#endif

HAL_StatusTypeDef OLED_Write_Byte(uint8_t, uint8_t);
void OLED_Fill(uint8_t);
void OLED_Refresh(void);
void OLED_SetPos(uint8_t, uint8_t);
HAL_StatusTypeDef OLED_BOOL_DrawColumn(uint8_t, uint8_t,  uint8_t*, uint8_t, uint8_t); // 以布尔运算方式绘制不定长列
HAL_StatusTypeDef OLED_BOOL_DrawChar(uint8_t, uint8_t, uint8_t, uint8_t); // 以布尔运算方式绘制字符
HAL_StatusTypeDef OLED_BOOL_DrawStr(uint8_t, uint8_t, uint8_t*, uint8_t); // 以布尔运算方式绘制字符串
HAL_StatusTypeDef OLED_BOOL_DrawBMP(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, uint8_t); // 以布尔运算方式绘制位图
HAL_StatusTypeDef OLED_DrawColumn(uint8_t, uint8_t, uint8_t*, uint8_t); // 直接绘制不定长列
HAL_StatusTypeDef OLED_DrawChar(uint8_t, uint8_t, uint8_t); // 直接绘制字符
HAL_StatusTypeDef OLED_DrawStr(uint8_t, uint8_t, uint8_t*); // 直接绘制字符串
HAL_StatusTypeDef OLED_DrawBMP(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*); // 直接绘制位图

#endif
