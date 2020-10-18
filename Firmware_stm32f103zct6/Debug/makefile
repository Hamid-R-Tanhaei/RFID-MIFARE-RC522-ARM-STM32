################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
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
Stm32f103zct6_RFID_Mifare_RC522.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Stm32f103zct6_RFID_Mifare_RC522.list \

OBJCOPY_HEX += \
Stm32f103zct6_RFID_Mifare_RC522.hex \


# All Target
all: Stm32f103zct6_RFID_Mifare_RC522.elf secondary-outputs

# Tool invocations
Stm32f103zct6_RFID_Mifare_RC522.elf: $(OBJS) $(USER_OBJS) D:\GitHub_MyRep\RFID_STM32\Firmware_stm32f103zct6\STM32F103ZCTX_FLASH.ld
	arm-none-eabi-gcc -o "Stm32f103zct6_RFID_Mifare_RC522.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\GitHub_MyRep\RFID_STM32\Firmware_stm32f103zct6\STM32F103ZCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Stm32f103zct6_RFID_Mifare_RC522.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Stm32f103zct6_RFID_Mifare_RC522.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Stm32f103zct6_RFID_Mifare_RC522.list"
	@echo 'Finished building: $@'
	@echo ' '

Stm32f103zct6_RFID_Mifare_RC522.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Stm32f103zct6_RFID_Mifare_RC522.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
