################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../InputHandler.cpp \
../Listable.cpp \
../Machines.cpp \
../Mask.cpp \
../MyVector.cpp \
../Object.cpp \
../PlayGround.cpp \
../constants.cpp \
../inputhandler.cpp 

OBJS += \
./InputHandler.o \
./Listable.o \
./Machines.o \
./Mask.o \
./MyVector.o \
./Object.o \
./PlayGround.o \
./constants.o \
./inputhandler.o 

CPP_DEPS += \
./InputHandler.d \
./Listable.d \
./Machines.d \
./Mask.d \
./MyVector.d \
./Object.d \
./PlayGround.d \
./constants.d \
./inputhandler.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


