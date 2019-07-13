################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/data_structs/DistributionTree.cpp \
../src/data_structs/MaskedBinaryRandomTree.cpp \
../src/data_structs/MyMaskedBinaryRandomTree.cpp \
../src/data_structs/RandomTree.cpp \
../src/data_structs/SimpleSet.cpp 

OBJS += \
./src/data_structs/DistributionTree.o \
./src/data_structs/MaskedBinaryRandomTree.o \
./src/data_structs/MyMaskedBinaryRandomTree.o \
./src/data_structs/RandomTree.o \
./src/data_structs/SimpleSet.o 

CPP_DEPS += \
./src/data_structs/DistributionTree.d \
./src/data_structs/MaskedBinaryRandomTree.d \
./src/data_structs/MyMaskedBinaryRandomTree.d \
./src/data_structs/RandomTree.d \
./src/data_structs/SimpleSet.d 


# Each subdirectory must supply rules for building sources it contributes
src/data_structs/%.o: ../src/data_structs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -llibboost_program_options  -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


