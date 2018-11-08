################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/src/base.c \
../Drivers/src/base_WS2812B.c \
../Drivers/src/base_blinky.c \
../Drivers/src/base_init.c \
../Drivers/src/base_pwm_tim1.c \
../Drivers/src/base_spi_1.c \
../Drivers/src/base_thread.c \
../Drivers/src/base_usart_1.c \
../Drivers/src/base_usb.c 

OBJS += \
./Drivers/src/base.o \
./Drivers/src/base_WS2812B.o \
./Drivers/src/base_blinky.o \
./Drivers/src/base_init.o \
./Drivers/src/base_pwm_tim1.o \
./Drivers/src/base_spi_1.o \
./Drivers/src/base_thread.o \
./Drivers/src/base_usart_1.o \
./Drivers/src/base_usb.o 

C_DEPS += \
./Drivers/src/base.d \
./Drivers/src/base_WS2812B.d \
./Drivers/src/base_blinky.d \
./Drivers/src/base_init.d \
./Drivers/src/base_pwm_tim1.d \
./Drivers/src/base_spi_1.d \
./Drivers/src/base_thread.d \
./Drivers/src/base_usart_1.d \
./Drivers/src/base_usb.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/src/%.o: ../Drivers/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DSTM32F103C8Tx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/inc" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/CMSIS/core" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/CMSIS/device" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/StdPeriph_Driver/inc" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/FreeRTOS/Source/include" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/BaseWithFreeRTOS/Drivers/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


