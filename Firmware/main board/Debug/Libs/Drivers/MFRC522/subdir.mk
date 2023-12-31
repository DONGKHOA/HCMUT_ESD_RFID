################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/Drivers/MFRC522/rc522.c 

OBJS += \
./Libs/Drivers/MFRC522/rc522.o 

C_DEPS += \
./Libs/Drivers/MFRC522/rc522.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/Drivers/MFRC522/%.o Libs/Drivers/MFRC522/%.su Libs/Drivers/MFRC522/%.cyclo: ../Libs/Drivers/MFRC522/%.c Libs/Drivers/MFRC522/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-Drivers-2f-MFRC522

clean-Libs-2f-Drivers-2f-MFRC522:
	-$(RM) ./Libs/Drivers/MFRC522/rc522.cyclo ./Libs/Drivers/MFRC522/rc522.d ./Libs/Drivers/MFRC522/rc522.o ./Libs/Drivers/MFRC522/rc522.su

.PHONY: clean-Libs-2f-Drivers-2f-MFRC522

