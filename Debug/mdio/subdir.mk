################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mdio/fsl_enet_mdio.c 

C_DEPS += \
./mdio/fsl_enet_mdio.d 

OBJS += \
./mdio/fsl_enet_mdio.o 


# Each subdirectory must supply rules for building sources it contributes
mdio/%.o: ../mdio/%.c mdio/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\source" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\mdio" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\phy" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\utilities" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\drivers" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\device" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\component\uart" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\component\lists" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\CMSIS" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\CMSIS_driver" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\board" -I"C:\Users\USER\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmk64f_cmsis_enet_txrx_transfer\frdmk64f\cmsis_driver_examples\enet\txrx_transfer" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-mdio

clean-mdio:
	-$(RM) ./mdio/fsl_enet_mdio.d ./mdio/fsl_enet_mdio.o

.PHONY: clean-mdio

