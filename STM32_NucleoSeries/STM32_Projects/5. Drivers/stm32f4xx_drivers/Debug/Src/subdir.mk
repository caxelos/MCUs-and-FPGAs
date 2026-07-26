################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/008spi_cmd_handling.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/008spi_cmd_handling.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/008spi_cmd_handling.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/008spi_cmd_handling.o: ../Src/008spi_cmd_handling.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -I"C:/Users/caxel/MCUs-and-FPGAs/STM32_NucleoSeries/STM32_Projects/5. Drivers/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/008spi_cmd_handling.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -I"C:/Users/caxel/MCUs-and-FPGAs/STM32_NucleoSeries/STM32_Projects/5. Drivers/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -I"C:/Users/caxel/MCUs-and-FPGAs/STM32_NucleoSeries/STM32_Projects/5. Drivers/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

