################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FOC/ekf.c \
../FOC/fast_math.c \
../FOC/foc.c 

OBJS += \
./FOC/ekf.o \
./FOC/fast_math.o \
./FOC/foc.o 

C_DEPS += \
./FOC/ekf.d \
./FOC/fast_math.d \
./FOC/foc.d 


# Each subdirectory must supply rules for building sources it contributes
FOC/%.o: ../FOC/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -fsingle-precision-constant -Wunused -Wuninitialized  -g -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\Debug" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\FOC" -I"E:\Desktop\单车\project\CH32V307VCT6\FOC" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\Core" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\User" -I"E:\Desktop\Bicycle\FOC_EKF\project\CH32V307VCT6\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

