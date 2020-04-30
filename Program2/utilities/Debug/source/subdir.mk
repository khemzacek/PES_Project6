################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/LED_control.c \
../source/Program2.c \
../source/delay.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/LED_control.o \
./source/Program2.o \
./source/delay.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/LED_control.d \
./source/Program2.d \
./source/delay.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=1 -DSDK_OS_FREE_RTOS -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\board" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\source" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\freertos" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\drivers" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\CMSIS" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\utilities" -I"C:\Users\Katherine.000\Documents\Spring 2020\PES\Project 6\workspace\Program2\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


