################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main.c \
../src/Team.c \
../src/conexiones.c \
../src/configTeam.c \
../src/entrenador.c \
../src/logTeam.c \
../src/movimiento.c \
../src/planificacion.c \
../src/utils.c 

OBJS += \
./src/Main.o \
./src/Team.o \
./src/conexiones.o \
./src/configTeam.o \
./src/entrenador.o \
./src/logTeam.o \
./src/movimiento.o \
./src/planificacion.o \
./src/utils.o 

C_DEPS += \
./src/Main.d \
./src/Team.d \
./src/conexiones.d \
./src/configTeam.d \
./src/entrenador.d \
./src/logTeam.d \
./src/movimiento.d \
./src/planificacion.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


