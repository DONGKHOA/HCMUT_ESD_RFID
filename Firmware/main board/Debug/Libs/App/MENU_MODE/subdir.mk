################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/App/MENU_MODE/menu_mode.c 

OBJS += \
./Libs/App/MENU_MODE/menu_mode.o 

C_DEPS += \
./Libs/App/MENU_MODE/menu_mode.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/App/MENU_MODE/%.o Libs/App/MENU_MODE/%.su Libs/App/MENU_MODE/%.cyclo: ../Libs/App/MENU_MODE/%.c Libs/App/MENU_MODE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-App-2f-MENU_MODE

clean-Libs-2f-App-2f-MENU_MODE:
	-$(RM) ./Libs/App/MENU_MODE/menu_mode.cyclo ./Libs/App/MENU_MODE/menu_mode.d ./Libs/App/MENU_MODE/menu_mode.o ./Libs/App/MENU_MODE/menu_mode.su

.PHONY: clean-Libs-2f-App-2f-MENU_MODE

