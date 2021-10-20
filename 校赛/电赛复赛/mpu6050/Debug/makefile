################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/subdir.mk
-include Core/state-space/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/OLED/subdir.mk
-include Core/DMP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
MPU-6050.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
MPU-6050.list \

OBJCOPY_HEX += \
MPU-6050.hex \

OBJCOPY_BIN += \
MPU-6050.bin \


# All Target
all: MPU-6050.elf secondary-outputs

# Tool invocations
MPU-6050.elf: $(OBJS) $(USER_OBJS) C:\Users\10970\STM32CubeIDE\workspace_1.4.0\MPU-6050\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "MPU-6050.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\10970\STM32CubeIDE\workspace_1.4.0\MPU-6050\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="MPU-6050.map" -Wl,--gc-sections -static -u_printf_float  -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

MPU-6050.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "MPU-6050.list"
	@echo 'Finished building: $@'
	@echo ' '

MPU-6050.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "MPU-6050.hex"
	@echo 'Finished building: $@'
	@echo ' '

MPU-6050.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "MPU-6050.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
