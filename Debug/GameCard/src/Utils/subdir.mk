################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GameCard/src/Utils/bitmap.c \
../GameCard/src/Utils/bloques.c \
../GameCard/src/Utils/conexion.c \
../GameCard/src/Utils/configGameCard.c \
../GameCard/src/Utils/files.c \
../GameCard/src/Utils/logGameCard.c \
../GameCard/src/Utils/metadata.c 

OBJS += \
./GameCard/src/Utils/bitmap.o \
./GameCard/src/Utils/bloques.o \
./GameCard/src/Utils/conexion.o \
./GameCard/src/Utils/configGameCard.o \
./GameCard/src/Utils/files.o \
./GameCard/src/Utils/logGameCard.o \
./GameCard/src/Utils/metadata.o 

C_DEPS += \
./GameCard/src/Utils/bitmap.d \
./GameCard/src/Utils/bloques.d \
./GameCard/src/Utils/conexion.d \
./GameCard/src/Utils/configGameCard.d \
./GameCard/src/Utils/files.d \
./GameCard/src/Utils/logGameCard.d \
./GameCard/src/Utils/metadata.d 


# Each subdirectory must supply rules for building sources it contributes
GameCard/src/Utils/%.o: ../GameCard/src/Utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


