################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Team/src/subdir.mk
-include Team/Debug/src/subdir.mk
-include GameCard/src/Utils/subdir.mk
-include GameCard/src/subdir.mk
-include GameCard/Debug/src/Utils/subdir.mk
-include GameCard/Debug/src/subdir.mk
-include GameBoy/src/Utils/subdir.mk
-include GameBoy/src/subdir.mk
-include GameBoy/Debug/src/Utils/subdir.mk
-include GameBoy/Debug/src/subdir.mk
-include Broker/src/Utils/subdir.mk
-include Broker/src/subdir.mk
-include Broker/Debug/src/Utils/subdir.mk
-include Broker/Debug/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Delibird

# Tool invocations
Delibird: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "Delibird" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Delibird
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
