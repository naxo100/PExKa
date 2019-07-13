################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/pattern/Channel.cpp \
../src/pattern/Compartment.cpp \
../src/pattern/Dependencies.cpp \
../src/pattern/Environment.cpp \
../src/pattern/RuleSet.cpp \
../src/pattern/Signature.cpp 

OBJS += \
./src/pattern/Channel.o \
./src/pattern/Compartment.o \
./src/pattern/Dependencies.o \
./src/pattern/Environment.o \
./src/pattern/RuleSet.o \
./src/pattern/Signature.o 

CPP_DEPS += \
./src/pattern/Channel.d \
./src/pattern/Compartment.d \
./src/pattern/Dependencies.d \
./src/pattern/Environment.d \
./src/pattern/RuleSet.d \
./src/pattern/Signature.d 


# Each subdirectory must supply rules for building sources it contributes
src/pattern/%.o: ../src/pattern/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options  -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


