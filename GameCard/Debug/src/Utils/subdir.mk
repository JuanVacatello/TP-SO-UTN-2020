################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Utils/bitmap.c \
../src/Utils/bloques.c \
../src/Utils/conexion.c \
../src/Utils/configGameCard.c \
../src/Utils/files.c \
../src/Utils/logGameCard.c \
../src/Utils/metadata.c 

OBJS += \
./src/Utils/bitmap.o \
./src/Utils/bloques.o \
./src/Utils/conexion.o \
./src/Utils/configGameCard.o \
./src/Utils/files.o \
./src/Utils/logGameCard.o \
./src/Utils/metadata.o 

C_DEPS += \
./src/Utils/bitmap.d \
./src/Utils/bloques.d \
./src/Utils/conexion.d \
./src/Utils/configGameCard.d \
./src/Utils/files.d \
./src/Utils/logGameCard.d \
./src/Utils/metadata.d 


# Each subdirectory must supply rules for building sources it contributes
src/Utils/%.o: ../src/Utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


