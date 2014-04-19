################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Comunication/Socket/ClientSocket.cpp \
../Comunication/Socket/ServerSocket.cpp \
../Comunication/Socket/Socket.cpp \
../Comunication/Socket/SocketInterface.cpp 

OBJS += \
./Comunication/Socket/ClientSocket.o \
./Comunication/Socket/ServerSocket.o \
./Comunication/Socket/Socket.o \
./Comunication/Socket/SocketInterface.o 

CPP_DEPS += \
./Comunication/Socket/ClientSocket.d \
./Comunication/Socket/ServerSocket.d \
./Comunication/Socket/Socket.d \
./Comunication/Socket/SocketInterface.d 


# Each subdirectory must supply rules for building sources it contributes
Comunication/Socket/%.o: ../Comunication/Socket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/rodrigo/tcc/toolchain/usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-g++ -I"/home/rodrigo/tcc/workspace/MainApp/headers" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


