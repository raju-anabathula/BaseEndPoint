################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TEST/EmpFullMessageTests.cpp \
../TEST/GtestMain.cpp \
../TEST/TestClassFramework.cpp \
../TEST/TestIPCTechniques.cpp 

OBJS += \
./TEST/EmpFullMessageTests.o \
./TEST/GtestMain.o \
./TEST/TestClassFramework.o \
./TEST/TestIPCTechniques.o 

CPP_DEPS += \
./TEST/EmpFullMessageTests.d \
./TEST/GtestMain.d \
./TEST/TestClassFramework.d \
./TEST/TestIPCTechniques.d 


# Each subdirectory must supply rules for building sources it contributes
TEST/%.o: ../TEST/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/BaseEndPoint" -I"/home/araju/workspace/BaseEndPoint/DYN_CONFIG/include" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -I/software/yst/linux/x86_64/boost/1.61.0/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


