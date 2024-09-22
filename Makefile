# Устанавливаем параметры компилятора
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AS = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Параметры компиляции
CFLAGS = -mcpu=cortex-m3 -mthumb -mthumb-interwork \
         -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 \
         -funroll-loops -fomit-frame-pointer -finline-functions -O0 -g -specs=nosys.specs

CXXFLAGS = -std=c++11
CFLAGS += -std=c11

# Включаем каталоги заголовков
INCLUDES = -I src -I Drivers/APM32F10x_StdPeriphDriver/inc -I Drivers/CMSIS/Include -I Drivers/Geehy/APM32F10x/Include

# Определения
DEFINES = -DAPM32F10X_MD \
           -DFIRMWARE_VERSION="0.0.1" \
           -DMCUID_ADDRESS=0x1ffff7e8 \

# Скрипт компоновщика
LINKER_SCRIPT = linker-script.ld

# Файлы исходного кода
SOURCES = $(wildcard src/*.*) $(wildcard Drivers/*.*) $(wildcard Lib/*.*)

# Имя проекта
PROJECT_NAME = adc-module
ELF_FILE = $(PROJECT_NAME).elf
HEX_FILE = $(PROJECT_NAME).hex
BIN_FILE = $(PROJECT_NAME).bin

# Правило сборки
all: $(ELF_FILE) $(HEX_FILE) $(BIN_FILE)

$(ELF_FILE): $(SOURCES) $(LINKER_SCRIPT)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -o $@ $(SOURCES) -T $(LINKER_SCRIPT) -Wl,--gc-sections,--print-memory-usage,-Map=$(PROJECT_NAME).map

$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -Oihex $(ELF_FILE) $@

$(BIN_FILE): $(ELF_FILE)
	$(OBJCOPY) -Obinary $(ELF_FILE) $@

clean:
	rm -f $(ELF_FILE) $(HEX_FILE) $(BIN_FILE) $(PROJECT_NAME).map
