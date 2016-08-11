################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../IPC_POSIX_MQ/PosixMessageQueue.cpp 

OBJS += \
./IPC_POSIX_MQ/PosixMessageQueue.o 

CPP_DEPS += \
./IPC_POSIX_MQ/PosixMessageQueue.d 


# Each subdirectory must supply rules for building sources it contributes
IPC_POSIX_MQ/%.o: ../IPC_POSIX_MQ/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/BaseEndPoint" -I"/home/araju/workspace/BaseEndPoint/DYN_CONFIG/include" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


