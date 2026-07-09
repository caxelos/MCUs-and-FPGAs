################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/STM32F407xx_gpio_driver.c \
../drivers/src/STM32F407xx_spi_driver.c 

OBJS += \
./drivers/src/STM32F407xx_gpio_driver.o \
./drivers/src/STM32F407xx_spi_driver.o 

C_DEPS += \
./drivers/src/STM32F407xx_gpio_driver.d \
./drivers/src/STM32F407xx_spi_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/STM32F407xx_gpio_driver.o: ../drivers/src/STM32F407xx_gpio_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -I"C:/Users/caxel/MCUs-and-FPGAs/STM32_NucleoSeries/STM32_Projects/5. Drivers/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/src/STM32F407xx_gpio_driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/src/STM32F407xx_spi_driver.o: ../drivers/src/STM32F407xx_spi_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -I"C:/Users/caxel/MCUs-and-FPGAs/STM32_NucleoSeries/STM32_Projects/5. Drivers/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/src/STM32F407xx_spi_driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

