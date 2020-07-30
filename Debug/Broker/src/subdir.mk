################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Broker/src/Broker.c 

OBJS += \
./Broker/src/Broker.o 

C_DEPS += \
./Broker/src/Broker.d 


# Each subdirectory must supply rules for building sources it contributes
Broker/src/%.o: ../Broker/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


