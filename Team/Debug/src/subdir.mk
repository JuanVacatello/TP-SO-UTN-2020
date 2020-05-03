################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Team.c \
../src/entrenador.c \
../src/log.c \
../src/movimiento.c 

OBJS += \
./src/Team.o \
./src/entrenador.o \
./src/log.o \
./src/movimiento.o 

C_DEPS += \
./src/Team.d \
./src/entrenador.d \
./src/log.d \
./src/movimiento.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


