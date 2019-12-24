################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/grammar/KappaDriver.cpp 

OBJS += \
./src/grammar/KappaDriver.o 

CPP_DEPS += \
./src/grammar/KappaDriver.d 


# Each subdirectory must supply rules for building sources it contributes
src/grammar/%.o: ../src/grammar/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options  -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


