################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Basic_parser.cpp \
../src/SPC_reader.cpp \
../src/Super_parser.cpp \
../src/TMULTI_TXVALS.cpp \
../src/TMULTI_parser.cpp \
../src/TXVALS_parser.cpp \
../src/main.cpp \
../src/rcpp_hello_world.cpp \
../src/rcpp_read_spc.cpp \
../src/test_wrapper.cpp 

OBJS += \
./src/Basic_parser.o \
./src/SPC_reader.o \
./src/Super_parser.o \
./src/TMULTI_TXVALS.o \
./src/TMULTI_parser.o \
./src/TXVALS_parser.o \
./src/main.o \
./src/rcpp_hello_world.o \
./src/rcpp_read_spc.o \
./src/test_wrapper.o 

CPP_DEPS += \
./src/Basic_parser.d \
./src/SPC_reader.d \
./src/Super_parser.d \
./src/TMULTI_TXVALS.d \
./src/TMULTI_parser.d \
./src/TXVALS_parser.d \
./src/main.d \
./src/rcpp_hello_world.d \
./src/rcpp_read_spc.d \
./src/test_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DINSIDE -I/home/simon/R/x86_64-pc-linux-gnu-library/2.15/Rcpp/include/Rcpp -I/home/simon/R/x86_64-pc-linux-gnu-library/2.15/Rcpp/include -I/home/simon/R/x86_64-pc-linux-gnu-library/2.15/RInside/include -I/usr/share/R/include -O0 -g3 -lm -pedantic -Wall -Wextra -c -fmessage-length=0 -shared -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


