################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Broker/src/Utils/configBroker.c \
../Broker/src/Utils/logBroker.c \
../Broker/src/Utils/memoria.c \
../Broker/src/Utils/servidor.c 

OBJS += \
./Broker/src/Utils/configBroker.o \
./Broker/src/Utils/logBroker.o \
./Broker/src/Utils/memoria.o \
./Broker/src/Utils/servidor.o 

C_DEPS += \
./Broker/src/Utils/configBroker.d \
./Broker/src/Utils/logBroker.d \
./Broker/src/Utils/memoria.d \
./Broker/src/Utils/servidor.d 


# Each subdirectory must supply rules for building sources it contributes
Broker/src/Utils/%.o: ../Broker/src/Utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


