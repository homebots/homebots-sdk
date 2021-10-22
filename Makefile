# Example of Makefile for any esp8266 project

FLASH_SPEED   ?= 230400
ESP_PORT      ?= $$(ls /dev/tty*usbserial*)
PROJECT       ?= $$PWD
ESPTOOL       ?= esptool.py

.PHONY: build flash asm sym bf

MK = docker run --rm -it -v$(PROJECT):/home/project -v$$PWD:/home/homebots-sdk -e LIBS -e VERBOSE homebotz/xtensa-gcc

build:
	$(MK) make

erase:
	$(ESPTOOL) --port $(ESP_PORT) erase_flash

flash:
	$(ESPTOOL) --after no_reset  --baud $(FLASH_SPEED) --port $(ESP_PORT) write_flash --compress --flash_freq 80m -fm qio -fs 1MB 0x00000 $(PROJECT)/firmware/0x00000.bin 0x10000 $(PROJECT)/firmware/0x10000.bin
# 0x7c000 $(PROJECT)/firmware/init_data.bin

asm:
	$(MK) make disassemble

sym:
	$(MK) make symboldump

sh:
	$(MK)

bf: build flash