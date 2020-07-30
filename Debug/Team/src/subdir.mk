################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Team/src/Main.c \
../Team/src/Team.c \
../Team/src/conexiones.c \
../Team/src/configTeam.c \
../Team/src/entrenador.c \
../Team/src/logTeam.c \
../Team/src/movimiento.c \
../Team/src/planificacion.c \
../Team/src/utils.c 

OBJS += \
./Team/src/Main.o \
./Team/src/Team.o \
./Team/src/conexiones.o \
./Team/src/configTeam.o \
./Team/src/entrenador.o \
./Team/src/logTeam.o \
./Team/src/movimiento.o \
./Team/src/planificacion.o \
./Team/src/utils.o 

C_DEPS += \
./Team/src/Main.d \
./Team/src/Team.d \
./Team/src/conexiones.d \
./Team/src/configTeam.d \
./Team/src/entrenador.d \
./Team/src/logTeam.d \
./Team/src/movimiento.d \
./Team/src/planificacion.d \
./Team/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Team/src/%.o: ../Team/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


