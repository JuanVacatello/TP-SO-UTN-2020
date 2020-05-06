################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/config.c \
../src/utils/log.c \
../src/utils/messageQueue.c \
../src/utils/utils.c 

OBJS += \
./src/utils/config.o \
./src/utils/log.o \
./src/utils/messageQueue.o \
./src/utils/utils.o 

C_DEPS += \
./src/utils/config.d \
./src/utils/log.d \
./src/utils/messageQueue.d \
./src/utils/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


