################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../General/Lock.cpp \
../General/Mutex.cpp \
../General/Thread.cpp 

OBJS += \
./General/Lock.o \
./General/Mutex.o \
./General/Thread.o 

CPP_DEPS += \
./General/Lock.d \
./General/Mutex.d \
./General/Thread.d 


# Each subdirectory must supply rules for building sources it contributes
General/%.o: ../General/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/rodrigo/tcc/toolchain/usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-g++ -I"/home/rodrigo/tcc/workspace/MainApp/headers" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


