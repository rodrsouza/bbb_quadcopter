################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../MainApp.cpp 

OBJS += \
./MainApp.o 

CPP_DEPS += \
./MainApp.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/rodrigo/tcc/toolchain/usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-g++ -I"/home/rodrigo/tcc/workspace/MainApp/headers" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


