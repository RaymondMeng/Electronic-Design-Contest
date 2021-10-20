################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/DMP/inv_mpu.c \
../Core/DMP/inv_mpu_dmp_motion_driver.c \
../Core/DMP/mpu6050.c 

OBJS += \
./Core/DMP/inv_mpu.o \
./Core/DMP/inv_mpu_dmp_motion_driver.o \
./Core/DMP/mpu6050.o 

C_DEPS += \
./Core/DMP/inv_mpu.d \
./Core/DMP/inv_mpu_dmp_motion_driver.d \
./Core/DMP/mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
Core/DMP/inv_mpu.o: ../Core/DMP/inv_mpu.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/DMP/inv_mpu.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/DMP/inv_mpu_dmp_motion_driver.o: ../Core/DMP/inv_mpu_dmp_motion_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/DMP/inv_mpu_dmp_motion_driver.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Core/DMP/mpu6050.o: ../Core/DMP/mpu6050.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/10970/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.2/Drivers/CMSIS/Include -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/DMP" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/state-space" -I"C:/Users/10970/STM32CubeIDE/workspace_1.4.0/MPU-6050/Core/OLED" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/DMP/mpu6050.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

