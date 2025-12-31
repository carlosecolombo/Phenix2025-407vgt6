#######################################
# Project
#######################################
PROJECT = Phenix2025-407VGT6
BUILD_DIR = build/debug

#######################################
# Toolchain
#######################################
CC  = arm-none-eabi-gcc
AS  = arm-none-eabi-gcc
SZ  = arm-none-eabi-size

#######################################
# MCU flags
#######################################
MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

#######################################
# Includes
#######################################
INCLUDES = \
-ICore/Inc \
-IDrivers/STM32F4xx_HAL_Driver/Inc \
-IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
-IDrivers/CMSIS/Include

#######################################
# Common C flags
#######################################
CFLAGS_COMMON = $(MCU) \
-DUSE_HAL_DRIVER \
-DSTM32F407xx \
-Og -g -gdwarf-2 \
-Wall -Wextra \
-ffunction-sections -fdata-sections

# Tu código
CFLAGS_USER = $(CFLAGS_COMMON)

# HAL (silencia unused-parameter)
CFLAGS_HAL  = $(CFLAGS_COMMON) -Wno-unused-parameter

#######################################
# ASM flags
#######################################
ASFLAGS = $(MCU)

#######################################
# Linker
#######################################
LDSCRIPT = STM32F407VGTX_FLASH.ld

LDFLAGS = $(MCU) \
-T$(LDSCRIPT) \
-Wl,--gc-sections \
--specs=nosys.specs \
-Wl,-Map=$(BUILD_DIR)/$(PROJECT).map

#######################################
# Sources
#######################################
CORE_SRC = \
Core/Src/main.c \
Core/Src/stm32f4xx_it.c \
Core/Src/system_stm32f4xx.c \
Core/Src/syscalls.c \
Core/Src/Comunicacion.c \
Core/Src/Conversor.c \
Core/Src/Drivers.c \
Core/Src/Flash-CCM-BackUp.c \
Core/Src/Libreria.c \
Core/Src/Test.c \
Core/Src/Testeos.c \
Core/Src/Trabajo.c

HAL_SRC = \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c

STARTUP = Core/Startup/startup_stm32f407vgtx.s

#######################################
# Objects
#######################################
OBJ_CORE = $(CORE_SRC:%.c=$(BUILD_DIR)/%.o)
OBJ_HAL  = $(HAL_SRC:%.c=$(BUILD_DIR)/%.o)
OBJ_STARTUP = $(BUILD_DIR)/$(STARTUP:.s=.o)

OBJECTS = $(OBJ_CORE) $(OBJ_HAL) $(OBJ_STARTUP)

#######################################
# Rules
#######################################
all: check

check: $(BUILD_DIR)/$(PROJECT).elf

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/Core/Src
	mkdir -p $(BUILD_DIR)/Drivers/STM32F4xx_HAL_Driver/Src
	mkdir -p $(BUILD_DIR)/Core/Startup

# -------- HAL --------
$(BUILD_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/%.o: Drivers/STM32F4xx_HAL_Driver/Src/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS_HAL) $(INCLUDES) -MMD -MP $< -o $@

# -------- TU CODIGO --------
$(BUILD_DIR)/Core/Src/%.o: Core/Src/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS_USER) $(INCLUDES) -MMD -MP $< -o $@

# -------- STARTUP --------
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

# -------- LINK --------
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

#######################################
# Clean
#######################################
clean:
	rm -rf build

#######################################
# Dependencies
#######################################
-include $(OBJECTS:.o=.d)
