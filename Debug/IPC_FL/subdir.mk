################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../IPC_FL/FileLocking.cpp 

OBJS += \
./IPC_FL/FileLocking.o 

CPP_DEPS += \
./IPC_FL/FileLocking.d 


# Each subdirectory must supply rules for building sources it contributes
IPC_FL/%.o: ../IPC_FL/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/BaseEndPoint" -I"/home/araju/workspace/BaseEndPoint/DYN_CONFIG/include" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


