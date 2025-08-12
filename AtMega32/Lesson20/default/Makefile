###############################################################################
# Makefile for the project Lesson20
###############################################################################

## General Flags
PROJECT = Lesson20
MCU = atmega32
TARGET = Lesson20.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Lesson20.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = KBDinclude.o LCDinclude.o Lesson20.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Lesson20.hex Lesson20.eep Lesson20.lss## Compile
KBDinclude.o: ../KBDinclude.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCDinclude.o: ../LCDinclude.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Lesson20.o: ../Lesson20.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Lesson20.elf dep/* Lesson20.hex Lesson20.eep Lesson20.lss Lesson20.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

