################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/Drivers/FLASH_MEMORY/flash_memory.c 

OBJS += \
./Libs/Drivers/FLASH_MEMORY/flash_memory.o 

C_DEPS += \
./Libs/Drivers/FLASH_MEMORY/flash_memory.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/Drivers/FLASH_MEMORY/%.o Libs/Drivers/FLASH_MEMORY/%.su Libs/Drivers/FLASH_MEMORY/%.cyclo: ../Libs/Drivers/FLASH_MEMORY/%.c Libs/Drivers/FLASH_MEMORY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-Drivers-2f-FLASH_MEMORY

clean-Libs-2f-Drivers-2f-FLASH_MEMORY:
	-$(RM) ./Libs/Drivers/FLASH_MEMORY/flash_memory.cyclo ./Libs/Drivers/FLASH_MEMORY/flash_memory.d ./Libs/Drivers/FLASH_MEMORY/flash_memory.o ./Libs/Drivers/FLASH_MEMORY/flash_memory.su

.PHONY: clean-Libs-2f-Drivers-2f-FLASH_MEMORY
