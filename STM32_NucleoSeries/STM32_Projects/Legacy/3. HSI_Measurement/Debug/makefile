################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
3.\ HSI_measurement.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
3.\ HSI_measurement.list \

OBJCOPY_BIN += \
3.\ HSI_measurement.bin \


# All Target
all: 3.\ HSI_measurement.elf secondary-outputs

# Tool invocations
3.\ HSI_measurement.elf: $(OBJS) $(USER_OBJS) C:\Users\caxel\MCUs-and-FPGAs\STM32_NucleoSeries\STM32_Projects\3.\ HSI_Measurement\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "3. HSI_measurement.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\caxel\MCUs-and-FPGAs\STM32_NucleoSeries\STM32_Projects\3. HSI_Measurement\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="3. HSI_measurement.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

3.\ HSI_measurement.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "3. HSI_measurement.list"
	@echo 'Finished building: $@'
	@echo ' '

3.\ HSI_measurement.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "3. HSI_measurement.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
