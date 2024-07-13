################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.c \
../Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.c \
../Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.c 

OBJS += \
./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.o \
./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.o \
./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.d \
./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.d \
./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/src/drivers/windows/%.o Middlewares/Third_Party/lvgl/src/drivers/windows/%.su Middlewares/Third_Party/lvgl/src/drivers/windows/%.cyclo: ../Middlewares/Third_Party/lvgl/src/drivers/windows/%.c Middlewares/Third_Party/lvgl/src/drivers/windows/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Middlewares/Third_Party/lvgl/src/drivers/display/ili9341" -I"D:/Embedded/RTED College/City_Dispatch_Sim/city_dispatch/Middlewares/Third_Party/lvgl" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-lvgl-2f-src-2f-drivers-2f-windows

clean-Middlewares-2f-Third_Party-2f-lvgl-2f-src-2f-drivers-2f-windows:
	-$(RM) ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.cyclo ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.d ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.o ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_context.su ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.cyclo ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.d ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.o ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_display.su ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.cyclo ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.d ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.o ./Middlewares/Third_Party/lvgl/src/drivers/windows/lv_windows_input.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-lvgl-2f-src-2f-drivers-2f-windows

