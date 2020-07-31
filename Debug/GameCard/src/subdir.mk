################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameCard/src/GameCard.c 

OBJS += \
./GameCard/src/GameCard.o 

C_DEPS += \
./GameCard/src/GameCard.d 


# Each subdirectory must supply rules for building sources it contributes
GameCard/src/%.o: ../GameCard/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


