################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/state/Node.cpp \
../src/state/SiteGraph.cpp \
../src/state/State.cpp \
../src/state/Variable.cpp 

OBJS += \
./src/state/Node.o \
./src/state/SiteGraph.o \
./src/state/State.o \
./src/state/Variable.o 

CPP_DEPS += \
./src/state/Node.d \
./src/state/SiteGraph.d \
./src/state/State.d \
./src/state/Variable.d 


# Each subdirectory must supply rules for building sources it contributes
src/state/%.o: ../src/state/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options  -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


