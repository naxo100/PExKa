################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/expressions/AlgExpression.cpp \
../src/expressions/BaseExpression.cpp \
../src/expressions/BinaryOperation.cpp \
../src/expressions/Constant.cpp \
../src/expressions/NullaryOperation.cpp \
../src/expressions/SomeValue.cpp \
../src/expressions/UnaryOperation.cpp \
../src/expressions/Vars.cpp 

OBJS += \
./src/expressions/AlgExpression.o \
./src/expressions/BaseExpression.o \
./src/expressions/BinaryOperation.o \
./src/expressions/Constant.o \
./src/expressions/NullaryOperation.o \
./src/expressions/SomeValue.o \
./src/expressions/UnaryOperation.o \
./src/expressions/Vars.o 

CPP_DEPS += \
./src/expressions/AlgExpression.d \
./src/expressions/BaseExpression.d \
./src/expressions/BinaryOperation.d \
./src/expressions/Constant.d \
./src/expressions/NullaryOperation.d \
./src/expressions/SomeValue.d \
./src/expressions/UnaryOperation.d \
./src/expressions/Vars.d 


# Each subdirectory must supply rules for building sources it contributes
src/expressions/%.o: ../src/expressions/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


