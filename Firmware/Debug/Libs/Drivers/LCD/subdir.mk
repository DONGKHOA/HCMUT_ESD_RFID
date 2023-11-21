################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/Drivers/LCD/lcd.c 

OBJS += \
./Libs/Drivers/LCD/lcd.o 

C_DEPS += \
./Libs/Drivers/LCD/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/Drivers/LCD/%.o Libs/Drivers/LCD/%.su Libs/Drivers/LCD/%.cyclo: ../Libs/Drivers/LCD/%.c Libs/Drivers/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/Drivers/KEYPAD" -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/Drivers/LCD" -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/Drivers/MFRC522" -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/App/DISPLAY_INFORMATION" -I"E:/Design Embedded System/HCMUT_ESD_RFID/Firmware/Libs/App/MENU_MODE" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-Drivers-2f-LCD

clean-Libs-2f-Drivers-2f-LCD:
	-$(RM) ./Libs/Drivers/LCD/lcd.cyclo ./Libs/Drivers/LCD/lcd.d ./Libs/Drivers/LCD/lcd.o ./Libs/Drivers/LCD/lcd.su

.PHONY: clean-Libs-2f-Drivers-2f-LCD

