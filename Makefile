#################### Global environment variables ####################
# Target name
TARGET_NAME     := msp430g2553

# Device name
DEVICE          := MSP430G2553

# Compiler directory path
GCC_DIR         := tools/msp430-compiler

# Compiler include directory path
GCC_MSP_INC_DIR := $(GCC_DIR)/include/msp

# Standard include directory path
GCC_STD_INC_DIR := $(GCC_DIR)/msp430-elf/include

# Linked file directory path
LD_DIR          := bsw/ld

# Compiler
CC              := @$(GCC_DIR)/bin/msp430-elf-gcc
GDB             := @$(GCC_DIR)/bin/msp430-elf-gdb
SIZE            := @$(GCC_DIR)/bin/msp430-elf-size
AR              := @$(GCC_DIR)/bin/msp430-elf-ar
LD              := @$(GCC_DIR)/bin/msp430-elf-ld
READ_ELF        := @$(GCC_DIR)/bin/msp430-elf-readelf
OBJ_DUMP        := @$(GCC_DIR)/bin/msp430-elf-objdump
OBJ_COPY        := @$(GCC_DIR)/bin/msp430-elf-objcopy

############################ Define list ############################
# Define list
DEFINES_LIST    :=      \
    DEPRECATED          \

DEFINES         := $(addprefix -D, $(DEFINES_LIST))
############################ Source list ############################
# C files list
C_SOURCES_LIST  :=      \
    bsw/config/src/project_cfg.c                \
    bsw/lib/circular/src/circular.c             \
    bsw/drivers/fls/src/fls.c                   \
    bsw/drivers/gpio/src/gpio.c                 \
    bsw/drivers/gpt/src/gpt.c                   \
    bsw/drivers/mcu/src/mcu.c                   \
    bsw/drivers/uart/src/uart.c                 \
    bsw/drivers/wdt/src/wdt.c                   \
    bsw/services/com/src/com.c                  \
    bsw/services/debouncing/src/debouncing.c    \
    bsw/services/debug/src/debug.c              \
    bsw/services/diag/src/diag.c                \
    bsw/services/log/src/log.c                  \
    bsw/services/nvm/src/nvm.c                  \
    bsw/services/os/src/os.c                    \
    bsw/services/timer/src/timer.c              \

############################ Include list ############################
# C include list
C_INCLUDES_LIST :=              \
    bsw/common                  \
    bsw/config/inc              \
    bsw/lib/circular/inc        \
    bsw/lib/circular/cfg        \
    bsw/drivers/fls/inc         \
    bsw/drivers/fls/cfg         \
    bsw/drivers/gpio/inc        \
    bsw/drivers/gpio/cfg        \
    bsw/drivers/gpt/inc         \
    bsw/drivers/gpt/cfg         \
    bsw/drivers/mcu/inc         \
    bsw/drivers/mcu/cfg         \
    bsw/drivers/uart/inc        \
    bsw/drivers/uart/cfg        \
    bsw/drivers/wdt/inc         \
    bsw/drivers/wdt/cfg         \
    bsw/services/com/inc        \
    bsw/services/com/cfg        \
    bsw/services/debouncing/inc \
    bsw/services/debouncing/cfg \
    bsw/services/debug/inc      \
    bsw/services/debug/cfg      \
    bsw/services/diag/inc       \
    bsw/services/diag/cfg       \
    bsw/services/log/inc        \
    bsw/services/log/cfg        \
    bsw/services/nvm/inc        \
    bsw/services/nvm/cfg        \
    bsw/services/os/inc         \
    bsw/services/os/cfg         \
    bsw/services/timer/inc      \
    bsw/services/timer/cfg      \

C_INCLUDES      := $(addprefix -I, $(C_INCLUDES_LIST))

INCLUDES        := $(C_INCLUDES) -I$(GCC_MSP_INC_DIR) -I$(GCC_STD_INC_DIR)

############################ Objects list ############################
# C objects list
C_OBJECTS       := $(addprefix build/, $(notdir $(C_SOURCES_LIST:.c=.o)))

# Search path for C sources
vpath %.c $(sort $(dir $(C_SOURCES_LIST)))

##################### Linked and libraries files #####################
# Linked files
LD_FILES        := $(LD_DIR)/$(shell echo $(DEVICE) | tr A-Z a-z).ld

# Libraries
LIB_FILES       := 

# Library directories list
LIB_DIR_LIST    :=      \
    lib/                \

LIB_DIRS        := $(addprefix -L, $(LIB_DIR_LIST)) -L$(GCC_MSP_INC_DIR)

############################### Flags ###############################
# C flags
CFLAGS          := -Os -D__$(DEVICE)__ -mmcu=$(DEVICE) -g -ffunction-sections -fdata-sections $(DEFINES) -Wall

# LD flags
LDFLAGS         := -T$(LD_FILES) $(LIB_DIRS) $(LIB_FILES) -mmcu=$(DEVICE) -g -Wl,--gc-sections -Wl,-Map=build/$(TARGET_NAME).map

############################### Commands ###############################
.PHONY: all clean build info debug

default:
	@make all --no-print-directory

clean:
	@rm -rf build

all:
	@make clean
	@make build -j
	@make info

build: build/$(TARGET_NAME).elf

build/%.o: %.c
	@mkdir -p build
	@echo Compiling $< ...
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

build/$(TARGET_NAME).elf: $(C_OBJECTS)
	@echo Linking objects and generating output binary ...
	$(CC) $(LDFLAGS) $(C_OBJECTS) -o $@

info: build/$(TARGET_NAME).elf
	$(READ_ELF) -a $< > build/$(TARGET_NAME)_reader.info
	$(OBJ_DUMP) -d $< > build/$(TARGET_NAME)_objdump.info
	$(OBJ_COPY) -O srec $< build/$(TARGET_NAME).srec
	$(SIZE) $<

debug: build/$(TARGET_NAME).elf
	$(GDB) $<
