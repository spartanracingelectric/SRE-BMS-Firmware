# GCC makefile for SRE-BMS-Firmware

SRCS  = ./Main/main.c
SRCS += ./Modules/Src/modCAN.c
SRCS += ./Modules/Src/modCommands.c
SRCS += ./Modules/Src/modConfig.c
SRCS += ./Modules/Src/modDelay.c
SRCS += ./Modules/Src/modDisplay.c
SRCS += ./Modules/Src/modEffect.c
SRCS += ./Modules/Src/modFlash.c
SRCS += ./Modules/Src/modOperationalState.c
SRCS += ./Modules/Src/modPowerElectronics.c
SRCS += ./Modules/Src/modPowerState.c
SRCS += ./Modules/Src/modStateOfCharge.c
SRCS += ./Modules/Src/modTerminal.c
SRCS += ./Modules/Src/modUART.c
SRCS += ./Modules/Src/report_status.c
SRCS += ./Modules/Src/safety_check.c
SRCS += ./Modules/Src/current_sense.c

SRCS += ./Drivers/SWDrivers/Src/driverSWADC128D818.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWADS1015.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWCC1101.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWDCDC.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWEMC2305.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWINA238.c
SRCS += ./Drivers/SWDrivers/Src/driverSWISL28022.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWLTC6803.c
SRCS += ./Drivers/SWDrivers/Src/driverSWLTC6804.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWMCP3221.c
#SRCS += ./Drivers/SWDrivers/Src/driverSWCAL6416.c
SRCS += ./Drivers/SWDrivers/Src/driverSWSHT21.c
SRCS += ./Drivers/SWDrivers/Src/driverSWSSD1306.c
SRCS += ./Drivers/SWDrivers/Src/driverSWStorageManager.c
SRCS += ./Drivers/SWDrivers/Src/driverSWUART2.c

SRCS += ./Drivers/HWDrivers/Src/driverHWADC.c
SRCS += ./Drivers/HWDrivers/Src/driverHWEEPROM.c
SRCS += ./Drivers/HWDrivers/Src/driverHWI2C1.c
SRCS += ./Drivers/HWDrivers/Src/driverHWI2C2.c
SRCS += ./Drivers/HWDrivers/Src/driverHWPowerState.c
SRCS += ./Drivers/HWDrivers/Src/driverHWSPI1.c
SRCS += ./Drivers/HWDrivers/Src/driverHWStatus.c
SRCS += ./Drivers/HWDrivers/Src/driverHWSwitches.c
SRCS += ./Drivers/HWDrivers/Src/driverHWUART2.c

SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc_ex.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_can.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_iwdg.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
SRCS += ./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c

SRCS += ./Device/stm32f3xx_it.c
SRCS += ./Device/stm32f3xx_hal_msp.c
SRCS += ./Drivers/CMSIS/Device/ST/STM32F3xx/Source/Templates/system_stm32f3xx.c

SRCS += ./Libraries/Scr/libBuffer.c
SRCS += ./Libraries/Scr/libCRC.c
SRCS += ./Libraries/Scr/libLogos.c
SRCS += ./Libraries/Scr/libGLCDFont.c
SRCS += ./Libraries/Scr/libGraphics.c
SRCS += ./Libraries/Scr/libPacket.c
SRCS += ./Libraries/Scr/libRingbuffer.c

SRCS += ./gcc/EBMS-startup.c

# TODO: place object files in separate directory
OBJS = $(SRCS:.c=.o)

INCLUDE  = -I ./Main
INCLUDE += -I ./Drivers/CMSIS/Device/ST/STM32F3xx/Include
INCLUDE += -I ./Drivers/CMSIS/Include
INCLUDE += -I ./Drivers/HWDrivers/Inc
INCLUDE += -I ./Drivers/STM32F3xx_HAL_Driver/Inc
INCLUDE += -I ./Drivers/SWDrivers/Inc
INCLUDE += -I ./Libraries/Inc
INCLUDE += -I ./Modules/Inc
INCLUDE += -I ./CubeMX/Inc

LINKER_SCRIPT = ./gcc/EBMS-linker.ld

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CFLAGS  = -std=gnu99 -g -Os -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork
CFLAGS += -mcpu=cortex-m4 -fsingle-precision-constant
CFLAGS += -Wdouble-promotion
CFLAGS += -specs=nano.specs -specs=nosys.specs
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard -march=armv7e-m
CFLAGS += -ffunction-sections -fdata-sections
# define board for compiler
CFLAGS += -D STM32F303xC
CFLAGS += -D USE_HAL_DRIVER

LINKER_FLAGS = -Wl,-Map=ENNOID-BMS.map -Wl,--gc-sections

OPENOCD_FLAGS = -f interface/stlink-v2.cfg -f target/stm32f3x.cfg

all: ENNOID-BMS.elf ENNOID-BMS.bin

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -lm

# $(OBJS): | build/
# $(OBJCOPY) -O binary $< $@

ENNOID-BMS.elf: $(OBJS)
	$(CC) -T$(LINKER_SCRIPT) $(CFLAGS) $(LINKER_FLAGS) $^ -o $@

ENNOID-BMS.bin: ENNOID-BMS.elf
	$(OBJCOPY) -O binary $< $@

upload-stlink: ENNOID-BMS.elf
	openocd $(OPENOCD_FLAGS) -c "program ENNOID-BMS.elf reset exit"

upload-bin-stlink: ENNOID-BMS.bin
	openocd $(OPENOCD_FLAGS) -c "program ENNOID-BMS.bin reset verify exit 0x08000000"

OPENOCD_JLINK_FLAGS = -f interface/jlink.cfg -f gcc/jlink.cfg -f target/stm32f3x.cfg
#upload: main.elf
	#openocd $(OPENOCD_JLINK_FLAGS) -c "init" -c "reset init" -c "flash write_image erase ENNOID-BMS.elf" -c "reset" -c "shutdown"

upload-bin: ENNOID-BMS.bin
	openocd $(OPENOCD_JLINK_FLAGS) -c "program ENNOID-BMS.bin reset verify exit 0x08000000"

connect:
	openocd $(OPENOCD_FLAGS)

debug:
	arm-none-eabi-gdb --eval-command="target remote localhost:3333" ENNOID-BMS.elf

clean:
	rm ENNOID-BMS.elf
	rm ENNOID-BMS.bin
	rm Main/main.o
	rm Modules/Src/*.o
	rm Libraries/Scr/*.o
	rm Drivers/HWDrivers/Src/*.o
	rm Drivers/SWDrivers/Src/*.o
	rm Drivers/STM32F3xx_HAL_Driver/Src/*.o
	rm Drivers/CMSIS/Device/ST/STM32F3xx/Source/Templates/*.o
	rm gcc/*.o