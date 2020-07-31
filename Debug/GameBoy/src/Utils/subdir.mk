################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameBoy/src/Utils/configGameBoy.c \
../GameBoy/src/Utils/logGameBoy.c \
../GameBoy/src/Utils/messageQueue.c 

OBJS += \
./GameBoy/src/Utils/configGameBoy.o \
./GameBoy/src/Utils/logGameBoy.o \
./GameBoy/src/Utils/messageQueue.o 

C_DEPS += \
./GameBoy/src/Utils/configGameBoy.d \
./GameBoy/src/Utils/logGameBoy.d \
./GameBoy/src/Utils/messageQueue.d 


# Each subdirectory must supply rules for building sources it contributes
GameBoy/src/Utils/%.o: ../GameBoy/src/Utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


