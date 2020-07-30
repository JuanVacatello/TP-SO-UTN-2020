################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Utils/configBroker.c \
../src/Utils/logBroker.c \
../src/Utils/memoria.c \
../src/Utils/servidor.c 

OBJS += \
./src/Utils/configBroker.o \
./src/Utils/logBroker.o \
./src/Utils/memoria.o \
./src/Utils/servidor.o 

C_DEPS += \
./src/Utils/configBroker.d \
./src/Utils/logBroker.d \
./src/Utils/memoria.d \
./src/Utils/servidor.d 


# Each subdirectory must supply rules for building sources it contributes
src/Utils/%.o: ../src/Utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


