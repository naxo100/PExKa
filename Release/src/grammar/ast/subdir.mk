################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/grammar/ast/AlgebraicExpressions.cpp \
../src/grammar/ast/Basics.cpp \
../src/grammar/ast/Dynamics.cpp \
../src/grammar/ast/KappaAst.cpp \
../src/grammar/ast/Spatial.cpp \
../src/grammar/ast/Statements.cpp 

OBJS += \
./src/grammar/ast/AlgebraicExpressions.o \
./src/grammar/ast/Basics.o \
./src/grammar/ast/Dynamics.o \
./src/grammar/ast/KappaAst.o \
./src/grammar/ast/Spatial.o \
./src/grammar/ast/Statements.o 

CPP_DEPS += \
./src/grammar/ast/AlgebraicExpressions.d \
./src/grammar/ast/Basics.d \
./src/grammar/ast/Dynamics.d \
./src/grammar/ast/KappaAst.d \
./src/grammar/ast/Spatial.d \
./src/grammar/ast/Statements.d 


# Each subdirectory must supply rules for building sources it contributes
src/grammar/ast/%.o: ../src/grammar/ast/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


