# darlanalves/homebots-sdk

Firmware SDK of all @homebots.

Build on top of [the ESP compiler](https://github.com/homebots/docker-espbuild)

## Usage

The container has everything to build and deploy firmware images for the ESP8266.

See some example projects [here](https://github.com/homebots/esp8266-starter-examples) or [here](https://github.com/esp8266/source-code-examples/blob/master/blinky/user/user_main.c)

## Project structure

A blank project looks like this:

```
-- src/
   index.c
-- Makefile
```

### src/index.c

```c
#include "esp-open-sdk.h"

void setup() {

}

void loop() {

}

```

### Makefile

```makefile
ESP_PORT       ?= /dev/ttyUSB0
FLASH_SPEED   ?= 230400

build:
	docker run --rm -it -v$$(pwd)/:/home/espbuilder/project darlanalves/espbuild

flash:
	esptool.py --baud $(FLASH_SPEED) --port $(ESP_PORT) write_flash -fm qio -fs 512KB 0x00000 firmware/0x00000.bin 0x10000 firmware/0x10000.bin 0x7c000 firmware/0x7c000.bin

```

### Build and flash

You will need `esptool.py` to flash your binaries.
It can be installed with `pip` or directly from GitHub:

```bash
pip2 install esptool

# if that does not work:

git clone https://github.com/espressif/esptool.git ~/esptool
cd ~/esptool
python setup.py install
```

Then, from a terminal, just run the `make` commands from your project's folder:

```
$ make build

CC project/src/index.c
AR project/build/esp8266_app.a
LD project/build/esp8266.out
FW project/firmware/
esptool.py v1.2

$ make flash

```

