################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/adc.c \
../User/ch32v30x_it.c \
../User/led.c \
../User/main.c \
../User/spi.c \
../User/system_ch32v30x.c \
../User/tim.c 

OBJS += \
./User/adc.o \
./User/ch32v30x_it.o \
./User/led.o \
./User/main.o \
./User/spi.o \
./User/system_ch32v30x.o \
./User/tim.o 

C_DEPS += \
./User/adc.d \
./User/ch32v30x_it.d \
./User/led.d \
./User/main.d \
./User/spi.d \
./User/system_ch32v30x.d \
./User/tim.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -fsingle-precision-constant -Wunused -Wuninitialized  -g -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\Debug" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\FOC" -I"E:\Desktop\单车\project\CH32V307VCT6\FOC" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\Core" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\User" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

