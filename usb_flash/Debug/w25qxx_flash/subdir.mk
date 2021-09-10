################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../w25qxx_flash/w25qxx.c 

OBJS += \
./w25qxx_flash/w25qxx.o 

C_DEPS += \
./w25qxx_flash/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
w25qxx_flash/w25qxx.o: ../w25qxx_flash/w25qxx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L053xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"w25qxx_flash/w25qxx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

