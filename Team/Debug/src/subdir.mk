################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Team.c \
../src/configTeam.c \
../src/entrenador.c \
../src/logTeam.c \
../src/movimiento.c \
../src/utils.c 

OBJS += \
./src/Team.o \
./src/configTeam.o \
./src/entrenador.o \
./src/logTeam.o \
./src/movimiento.o \
./src/utils.o 

C_DEPS += \
./src/Team.d \
./src/configTeam.d \
./src/entrenador.d \
./src/logTeam.d \
./src/movimiento.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


