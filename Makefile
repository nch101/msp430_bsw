#################### Global environment variables ####################
# Target name
TARGET_NAME     := msp430g2553

# Device name
DEVICE          := MSP430G2553

# Compiler directory path
GCC_DIR         := $(patsubst %/bin/msp430-elf-gcc,%,$(shell which msp430-elf-gcc))

# Compiler include directory path
GCC_MSP_INC_DIR := $(GCC_DIR)/include/msp

# Standard include directory path
GCC_STD_INC_DIR := $(GCC_DIR)/msp430-elf/include

# Libraries directory path
LIB_DIR         := lib/

# Compiler
CC              := msp430-elf-gcc
GDB             := msp430-elf-gdb
SIZE            := msp430-elf-size
AR              := msp430-elf-ar
LD              := msp430-elf-ld

############################ Source list ############################
# C files list
C_SOURCES_LIST  :=      \
    main.c              \
    bsw/driver/gpt/src/gpt.c    \
    bsw/driver/mcu/src/mcu.c    \
    bsw/driver/uart/src/uart.c  \

############################ Include list ############################
# C include list
C_INCLUDES_LIST :=      \
    bsw/common          \
    bsw/driver/gpt/inc  \
    bsw/driver/gpt/cfg  \
    bsw/driver/mcu/inc  \
    bsw/driver/mcu/cfg  \
    bsw/driver/uart/inc \
    bsw/driver/uart/cfg \

C_INCLUDES      := $(addprefix -I, $(C_INCLUDES_LIST))

INCLUDES        := $(C_INCLUDES) -I$(GCC_MSP_INC_DIR) -I$(GCC_STD_INC_DIR)

############################ Objects list ############################
# C objects list
C_OBJECTS       := $(addprefix build/, $(notdir $(C_SOURCES_LIST:.c=.o)))

# Search path for C sources
vpath %.c $(sort $(dir $(C_SOURCES_LIST)))

##################### Linked and libraries files #####################
# Linked files
LD_FILES        := $(GCC_MSP_INC_DIR)/$(shell echo $(DEVICE) | tr A-Z a-z).ld

# Libraries
LIB_FILES       := 

############################### Flags ###############################
# C flags
CFLAGS          := -Os -D__$(DEVICE)__ -mmcu=$(DEVICE) -g -ffunction-sections -fdata-sections -DDEPRECATED

# LD flags
LDFLAGS         := -T$(LD_FILES) -L$(GCC_MSP_INC_DIR) $(LIB_FILES) -mmcu=$(DEVICE) -g -Wl,--gc-sections -Wl,-Map=build/$(TARGET_NAME).map

############################### Commands ###############################
.PHONY: all clean build size debug

default:
	@make all --no-print-directory

clean:
	@rm -rf build

all:
	@make clean
	@make build -j
	@make size

build: build/$(TARGET_NAME).out

build/%.o: %.c
	@mkdir -p build
	@echo Compiling $< ...
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

build/$(TARGET_NAME).out: $(C_OBJECTS)
	@echo Linking objects and generating output binary ...
	$(CC) $(LDFLAGS) $(C_OBJECTS) -o $@

size: build
	$(SIZE) build/$(TARGET_NAME).out

debug: build
	$(GDB) build/$(TARGET_NAME).out
