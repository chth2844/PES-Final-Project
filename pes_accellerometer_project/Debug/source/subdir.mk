################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/i2c.c \
../source/main.c \
../source/mma8451.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/state_machine.c \
../source/systick.c \
../source/tpm.c 

OBJS += \
./source/i2c.o \
./source/main.o \
./source/mma8451.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/state_machine.o \
./source/systick.o \
./source/tpm.o 

C_DEPS += \
./source/i2c.d \
./source/main.d \
./source/mma8451.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/state_machine.d \
./source/systick.d \
./source/tpm.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\board" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\source" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\drivers" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\CMSIS" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\utilities" -I"C:\Users\chira\Documents\MCUXpressoIDE_11.2.0_4120\workspace\pes_accellerometer_project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


