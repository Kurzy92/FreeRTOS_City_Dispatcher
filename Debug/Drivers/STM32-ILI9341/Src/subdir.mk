################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32-ILI9341/Src/gpio.c \
../Drivers/STM32-ILI9341/Src/main.c \
../Drivers/STM32-ILI9341/Src/rng.c \
../Drivers/STM32-ILI9341/Src/spi.c \
../Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.c \
../Drivers/STM32-ILI9341/Src/stm32f7xx_it.c \
../Drivers/STM32-ILI9341/Src/tim.c \
../Drivers/STM32-ILI9341/Src/usart.c 

OBJS += \
./Drivers/STM32-ILI9341/Src/gpio.o \
./Drivers/STM32-ILI9341/Src/main.o \
./Drivers/STM32-ILI9341/Src/rng.o \
./Drivers/STM32-ILI9341/Src/spi.o \
./Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.o \
./Drivers/STM32-ILI9341/Src/stm32f7xx_it.o \
./Drivers/STM32-ILI9341/Src/tim.o \
./Drivers/STM32-ILI9341/Src/usart.o 

C_DEPS += \
./Drivers/STM32-ILI9341/Src/gpio.d \
./Drivers/STM32-ILI9341/Src/main.d \
./Drivers/STM32-ILI9341/Src/rng.d \
./Drivers/STM32-ILI9341/Src/spi.d \
./Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.d \
./Drivers/STM32-ILI9341/Src/stm32f7xx_it.d \
./Drivers/STM32-ILI9341/Src/tim.d \
./Drivers/STM32-ILI9341/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32-ILI9341/Src/%.o Drivers/STM32-ILI9341/Src/%.su Drivers/STM32-ILI9341/Src/%.cyclo: ../Drivers/STM32-ILI9341/Src/%.c Drivers/STM32-ILI9341/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Drivers" -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Drivers/STM32-ILI9341/Inc" -I../Core/Inc -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Drivers/lvgl" -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Application/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32-2d-ILI9341-2f-Src

clean-Drivers-2f-STM32-2d-ILI9341-2f-Src:
	-$(RM) ./Drivers/STM32-ILI9341/Src/gpio.cyclo ./Drivers/STM32-ILI9341/Src/gpio.d ./Drivers/STM32-ILI9341/Src/gpio.o ./Drivers/STM32-ILI9341/Src/gpio.su ./Drivers/STM32-ILI9341/Src/main.cyclo ./Drivers/STM32-ILI9341/Src/main.d ./Drivers/STM32-ILI9341/Src/main.o ./Drivers/STM32-ILI9341/Src/main.su ./Drivers/STM32-ILI9341/Src/rng.cyclo ./Drivers/STM32-ILI9341/Src/rng.d ./Drivers/STM32-ILI9341/Src/rng.o ./Drivers/STM32-ILI9341/Src/rng.su ./Drivers/STM32-ILI9341/Src/spi.cyclo ./Drivers/STM32-ILI9341/Src/spi.d ./Drivers/STM32-ILI9341/Src/spi.o ./Drivers/STM32-ILI9341/Src/spi.su ./Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.cyclo ./Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.d ./Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.o ./Drivers/STM32-ILI9341/Src/stm32f7xx_hal_msp.su ./Drivers/STM32-ILI9341/Src/stm32f7xx_it.cyclo ./Drivers/STM32-ILI9341/Src/stm32f7xx_it.d ./Drivers/STM32-ILI9341/Src/stm32f7xx_it.o ./Drivers/STM32-ILI9341/Src/stm32f7xx_it.su ./Drivers/STM32-ILI9341/Src/tim.cyclo ./Drivers/STM32-ILI9341/Src/tim.d ./Drivers/STM32-ILI9341/Src/tim.o ./Drivers/STM32-ILI9341/Src/tim.su ./Drivers/STM32-ILI9341/Src/usart.cyclo ./Drivers/STM32-ILI9341/Src/usart.d ./Drivers/STM32-ILI9341/Src/usart.o ./Drivers/STM32-ILI9341/Src/usart.su

.PHONY: clean-Drivers-2f-STM32-2d-ILI9341-2f-Src

