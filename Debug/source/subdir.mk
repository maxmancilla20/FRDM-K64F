################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c \
../source/queuelib.c \
../source/semihost_hardfault.c \
../source/tasks.c 

C_DEPS += \
./source/main.d \
./source/queuelib.d \
./source/semihost_hardfault.d \
./source/tasks.d 

OBJS += \
./source/main.o \
./source/queuelib.o \
./source/semihost_hardfault.o \
./source/tasks.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch\board" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch\source" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch\drivers" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch\CMSIS" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch\utilities" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\ContextSwitch\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/main.d ./source/main.o ./source/queuelib.d ./source/queuelib.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/tasks.d ./source/tasks.o

.PHONY: clean-source

