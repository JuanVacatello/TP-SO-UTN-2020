################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Utils/config.c \
../src/Utils/log.c \
../src/Utils/messageQueue.c 

OBJS += \
./src/Utils/config.o \
./src/Utils/log.o \
./src/Utils/messageQueue.o 

C_DEPS += \
./src/Utils/config.d \
./src/Utils/log.d \
./src/Utils/messageQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/Utils/%.o: ../src/Utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


