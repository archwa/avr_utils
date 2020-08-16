.PHONY: all clean program target target_spi

src ?= blink
prg_mmcu ?= atmega328p
prg_avrdude_p ?= m328p
target_mmcu ?= atmega168
ttyUSB ?= 0
SRC_DIR = src
INC_DIR = inc

all: program

# PROGRAM THE PROGRAMMER - TODO: make programmer a USART to SPI controller
program: $(src).hex $(src).bin
	avrdude -c arduino -p $(prg_avrdude_p) -b 57600 -P /dev/ttyUSB$(ttyUSB) -U flash:w:$<

$(src).hex: $(src).elf
	avr-objcopy -O ihex $< $@

$(src).bin: $(src).elf
	avr-objcopy -O binary $< $@

$(src).elf: $(src).o
	avr-gcc -mmcu=$(prg_mmcu) $< -o $@ 

$(src).o: $(SRC_DIR)/$(src).c
	avr-gcc -Os -mmcu=$(prg_mmcu) -I$(INC_DIR) -I. -c $<

# TARGET & SPI DIRECTIVE CREATION 
target: $(SRC_DIR)/target.c
	avr-gcc -Os -mmcu=$(target_mmcu) -c $<
	avr-gcc -Os -mmcu=$(target_mmcu) $< -o $@.elf
	avr-objcopy -O binary $@.elf $@.bin
	xxd -p $@.bin > $@.hex
	xxd -i $@.bin > $@_bin.h

create_spi.out: $(SRC_DIR)/create_spi.c target
	gcc -I. $< -o $@

target_spi: create_spi.out
	./$< > $@.hex
	xxd -r -p $@.hex > $@.bin
	xxd -i $@.bin > $@_bin.h

# CLEAN BUILT FILES
clean:
	rm -f *.bin *.elf *.h *.hex *.o *.out
	rm -f flash/*.bin
