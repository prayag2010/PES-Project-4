################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/PES\ Project\ 4.c \
../source/System.c \
../source/i2c.c \
../source/ledControl.c \
../source/loggerFunctions.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/stateFunctions.c \
../source/unitTest.c 

OBJS += \
./source/PES\ Project\ 4.o \
./source/System.o \
./source/i2c.o \
./source/ledControl.o \
./source/loggerFunctions.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/stateFunctions.o \
./source/unitTest.o 

C_DEPS += \
./source/PES\ Project\ 4.d \
./source/System.d \
./source/i2c.d \
./source/ledControl.d \
./source/loggerFunctions.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/stateFunctions.d \
./source/unitTest.d 


# Each subdirectory must supply rules for building sources it contributes
source/PES\ Project\ 4.o: ../source/PES\ Project\ 4.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VFM4 -Dnormal -DCPU_MKL25Z128VFM4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I../board -I../source -I../ -I../startup -I../CMSIS -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"source/PES Project 4.d" -MT"source/PES\ Project\ 4.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VFM4 -Dnormal -DCPU_MKL25Z128VFM4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I../board -I../source -I../ -I../startup -I../CMSIS -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


