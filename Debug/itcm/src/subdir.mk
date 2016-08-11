################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../itcm/src/ByteStream.cpp \
../itcm/src/InvalidMessage.cpp \
../itcm/src/ItcmException.cpp \
../itcm/src/NotNullPointer.cpp \
../itcm/src/NullPointer.cpp \
../itcm/src/OutputBufferDesc.cpp \
../itcm/src/OutputStringDesc.cpp \
../itcm/src/PreconditionFailed.cpp \
../itcm/src/ValueOutOfRange.cpp 

OBJS += \
./itcm/src/ByteStream.o \
./itcm/src/InvalidMessage.o \
./itcm/src/ItcmException.o \
./itcm/src/NotNullPointer.o \
./itcm/src/NullPointer.o \
./itcm/src/OutputBufferDesc.o \
./itcm/src/OutputStringDesc.o \
./itcm/src/PreconditionFailed.o \
./itcm/src/ValueOutOfRange.o 

CPP_DEPS += \
./itcm/src/ByteStream.d \
./itcm/src/InvalidMessage.d \
./itcm/src/ItcmException.d \
./itcm/src/NotNullPointer.d \
./itcm/src/NullPointer.d \
./itcm/src/OutputBufferDesc.d \
./itcm/src/OutputStringDesc.d \
./itcm/src/PreconditionFailed.d \
./itcm/src/ValueOutOfRange.d 


# Each subdirectory must supply rules for building sources it contributes
itcm/src/%.o: ../itcm/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/BaseEndPoint" -I"/home/araju/workspace/BaseEndPoint/DYN_CONFIG/include" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -I/software/yst/linux/x86_64/boost/1.61.0/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


