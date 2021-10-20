################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/retarget.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/retarget.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/retarget.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/gpio.o: ../Core/Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gpio.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/i2c.o: ../Core/Src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/i2c.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/retarget.o: ../Core/Src/retarget.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/retarget.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/spi.o: ../Core/Src/spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/spi.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/stm32f1xx_hal_msp.o: ../Core/Src/stm32f1xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f1xx_hal_msp.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/stm32f1xx_it.o: ../Core/Src/stm32f1xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f1xx_it.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/system_stm32f1xx.o: ../Core/Src/system_stm32f1xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32f1xx.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/tim.o: ../Core/Src/tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/tim.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/Src/usart.o: ../Core/Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/usart.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

