################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/choll/Entwicklung/rosc/rosc_node/src/rosc/string_res/msg_strings.c 

OBJS += \
./external_sources/rosc_node_src/rosc/string_res/msg_strings.o 

C_DEPS += \
./external_sources/rosc_node_src/rosc/string_res/msg_strings.d 


# Each subdirectory must supply rules for building sources it contributes
external_sources/rosc_node_src/rosc/string_res/msg_strings.o: /home/choll/Entwicklung/rosc/rosc_node/src/rosc/string_res/msg_strings.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__DEBUG__PRINTS__ -I/opt/ros/groovy/include -I/usr/include -I/usr/include/x86_64-linux-gnu -I/usr/lib/gcc/x86_64-linux-gnu/4.7/include -I"/home/choll/Entwicklung/rosc/rosc_node/include" -I"/home/choll/Entwicklung/rosc/rosc_example_messages/include" -I"/home/choll/Entwicklung/rosc/rosc_sys_linux_x86/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


