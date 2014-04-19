################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Comunication/cppsocket/address.cpp \
../Comunication/cppsocket/semaphore.cpp \
../Comunication/cppsocket/socket.cpp \
../Comunication/cppsocket/stringbuffer.cpp \
../Comunication/cppsocket/tcpclient.cpp \
../Comunication/cppsocket/tcpconnection.cpp \
../Comunication/cppsocket/tcpserver.cpp \
../Comunication/cppsocket/tcpthreadedserver.cpp \
../Comunication/cppsocket/thread.cpp \
../Comunication/cppsocket/udpnode.cpp 

OBJS += \
./Comunication/cppsocket/address.o \
./Comunication/cppsocket/semaphore.o \
./Comunication/cppsocket/socket.o \
./Comunication/cppsocket/stringbuffer.o \
./Comunication/cppsocket/tcpclient.o \
./Comunication/cppsocket/tcpconnection.o \
./Comunication/cppsocket/tcpserver.o \
./Comunication/cppsocket/tcpthreadedserver.o \
./Comunication/cppsocket/thread.o \
./Comunication/cppsocket/udpnode.o 

CPP_DEPS += \
./Comunication/cppsocket/address.d \
./Comunication/cppsocket/semaphore.d \
./Comunication/cppsocket/socket.d \
./Comunication/cppsocket/stringbuffer.d \
./Comunication/cppsocket/tcpclient.d \
./Comunication/cppsocket/tcpconnection.d \
./Comunication/cppsocket/tcpserver.d \
./Comunication/cppsocket/tcpthreadedserver.d \
./Comunication/cppsocket/thread.d \
./Comunication/cppsocket/udpnode.d 


# Each subdirectory must supply rules for building sources it contributes
Comunication/cppsocket/%.o: ../Comunication/cppsocket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/rodrigo/tcc/toolchain/usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-g++ -I"/home/rodrigo/tcc/workspace/MainApp/headers" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


