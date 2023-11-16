################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/keypad/key_pad_basic.c 

OBJS += \
./Libs/keypad/key_pad_basic.o 

C_DEPS += \
./Libs/keypad/key_pad_basic.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/keypad/%.o Libs/keypad/%.su Libs/keypad/%.cyclo: ../Libs/keypad/%.c Libs/keypad/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/RC522" -I"C:/STM32/HCMUT_ESD_RFID/Firmware/Libs/keypad" -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/keypad" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-keypad

clean-Libs-2f-keypad:
	-$(RM) ./Libs/keypad/key_pad_basic.cyclo ./Libs/keypad/key_pad_basic.d ./Libs/keypad/key_pad_basic.o ./Libs/keypad/key_pad_basic.su

.PHONY: clean-Libs-2f-keypad

