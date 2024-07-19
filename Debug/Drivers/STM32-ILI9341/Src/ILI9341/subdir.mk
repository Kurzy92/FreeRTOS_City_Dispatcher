################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.c \
../Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.c \
../Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.c 

OBJS += \
./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.o \
./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.o \
./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.o 

C_DEPS += \
./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.d \
./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.d \
./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32-ILI9341/Src/ILI9341/%.o Drivers/STM32-ILI9341/Src/ILI9341/%.su Drivers/STM32-ILI9341/Src/ILI9341/%.cyclo: ../Drivers/STM32-ILI9341/Src/ILI9341/%.c Drivers/STM32-ILI9341/Src/ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Drivers" -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Drivers/STM32-ILI9341/Inc" -I../Core/Inc -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Drivers/lvgl" -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Application/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32-2d-ILI9341-2f-Src-2f-ILI9341

clean-Drivers-2f-STM32-2d-ILI9341-2f-Src-2f-ILI9341:
	-$(RM) ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.cyclo ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.d ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.o ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_GFX.su ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.cyclo ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.d ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.o ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_STM32_Driver.su ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.cyclo ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.d ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.o ./Drivers/STM32-ILI9341/Src/ILI9341/ILI9341_Touchscreen.su

.PHONY: clean-Drivers-2f-STM32-2d-ILI9341-2f-Src-2f-ILI9341

