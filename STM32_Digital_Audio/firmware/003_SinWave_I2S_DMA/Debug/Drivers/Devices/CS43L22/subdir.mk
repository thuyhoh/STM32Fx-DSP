################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Devices/CS43L22/cs43l22.c 

OBJS += \
./Drivers/Devices/CS43L22/cs43l22.o 

C_DEPS += \
./Drivers/Devices/CS43L22/cs43l22.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Devices/CS43L22/%.o Drivers/Devices/CS43L22/%.su Drivers/Devices/CS43L22/%.cyclo: ../Drivers/Devices/CS43L22/%.c Drivers/Devices/CS43L22/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/Embedded_MCU/STM32/STM32F40x/STM32-DSP/STM32_Digital_Audio/firmware/003_SinWave_I2S_DMA/Drivers/Devices/CS43L22" -I"D:/Embedded_MCU/STM32/STM32F40x/STM32-DSP/STM32_Digital_Audio/firmware/003_SinWave_I2S_DMA/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Devices-2f-CS43L22

clean-Drivers-2f-Devices-2f-CS43L22:
	-$(RM) ./Drivers/Devices/CS43L22/cs43l22.cyclo ./Drivers/Devices/CS43L22/cs43l22.d ./Drivers/Devices/CS43L22/cs43l22.o ./Drivers/Devices/CS43L22/cs43l22.su

.PHONY: clean-Drivers-2f-Devices-2f-CS43L22

