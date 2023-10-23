################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/RC522/rc522.c 

OBJS += \
./Libs/RC522/rc522.o 

C_DEPS += \
./Libs/RC522/rc522.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/RC522/%.o Libs/RC522/%.su Libs/RC522/%.cyclo: ../Libs/RC522/%.c Libs/RC522/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/RC522" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-RC522

clean-Libs-2f-RC522:
	-$(RM) ./Libs/RC522/rc522.cyclo ./Libs/RC522/rc522.d ./Libs/RC522/rc522.o ./Libs/RC522/rc522.su

.PHONY: clean-Libs-2f-RC522

