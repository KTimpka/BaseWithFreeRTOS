################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/MemMang/heap_1.c 

OBJS += \
./FreeRTOS/Source/portable/MemMang/heap_1.o 

C_DEPS += \
./FreeRTOS/Source/portable/MemMang/heap_1.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/MemMang/%.o: ../FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DSTM32F103C8Tx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/inc" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/CMSIS/core" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/CMSIS/device" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/StdPeriph_Driver/inc" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/FreeRTOS/Source/include" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"C:/Users/Sad/workspace/BaseWithFreeRTOS/Drivers/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


