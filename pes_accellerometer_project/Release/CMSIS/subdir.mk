################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\board" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\source" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\drivers" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\CMSIS" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\utilities" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\startup" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

