################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 

OBJS += \
./component/lists/fsl_component_generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\source" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\utilities" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\drivers" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\device" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\component\uart" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\component\lists" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\CMSIS" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\board" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_gpio_input_interrupt_Swich button\frdmk64f\driver_examples\gpio\input_interrupt" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/fsl_component_generic_list.d ./component/lists/fsl_component_generic_list.o

.PHONY: clean-component-2f-lists

