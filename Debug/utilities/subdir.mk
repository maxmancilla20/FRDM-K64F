################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_str.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\source" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\drivers" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\utilities" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\device" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\component\uart" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\component\serial_manager" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\component\lists" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\CMSIS" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\freertos\freertos_kernel\include" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_freertos_event\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

