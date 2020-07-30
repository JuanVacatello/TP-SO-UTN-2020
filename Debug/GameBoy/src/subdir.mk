################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameBoy/src/GameBoy.c 

OBJS += \
./GameBoy/src/GameBoy.o 

C_DEPS += \
./GameBoy/src/GameBoy.d 


# Each subdirectory must supply rules for building sources it contributes
GameBoy/src/%.o: ../GameBoy/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


