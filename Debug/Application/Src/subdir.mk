################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Src/LCDController.c \
../Application/Src/dispatcher.c \
../Application/Src/task_handling.c 

OBJS += \
./Application/Src/LCDController.o \
./Application/Src/dispatcher.o \
./Application/Src/task_handling.o 

C_DEPS += \
./Application/Src/LCDController.d \
./Application/Src/dispatcher.d \
./Application/Src/task_handling.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Src/%.o Application/Src/%.su Application/Src/%.cyclo: ../Application/Src/%.c Application/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32F746_NUCLEO -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Application/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Src

clean-Application-2f-Src:
	-$(RM) ./Application/Src/LCDController.cyclo ./Application/Src/LCDController.d ./Application/Src/LCDController.o ./Application/Src/LCDController.su ./Application/Src/dispatcher.cyclo ./Application/Src/dispatcher.d ./Application/Src/dispatcher.o ./Application/Src/dispatcher.su ./Application/Src/task_handling.cyclo ./Application/Src/task_handling.d ./Application/Src/task_handling.o ./Application/Src/task_handling.su

.PHONY: clean-Application-2f-Src

