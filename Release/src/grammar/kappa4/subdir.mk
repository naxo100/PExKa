################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/grammar/kappa4/Lexer.cpp \
../src/grammar/kappa4/Parser.cpp 

OBJS += \
./src/grammar/kappa4/Lexer.o \
./src/grammar/kappa4/Parser.o 

CPP_DEPS += \
./src/grammar/kappa4/Lexer.d \
./src/grammar/kappa4/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/grammar/kappa4/%.o: ../src/grammar/kappa4/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options  -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


