# Extensions to ESP Non-OS SDK

These are the additions to the [xtensa-gcc image](https://github.com/homebots/docker-xtensa-gcc)

## Usage

See some example projects [here](https://github.com/homebots/esp8266-examples) or [here](https://github.com/esp8266/source-code-examples/blob/master/blinky/user/user_main.c)

## Project structure

A project looks like this:

```
-- Makefile
-- src/
 |-- index.cpp
 |
-- includes/
 |-- lib.h
 |-- other-lib.h

```

You can check the tests folder for samples of how to use the project.

### Build and flash

You will need `esptool.py` to flash your binaries.
It can be installed with `pip` or directly from GitHub:

```bash
# using pip or pip3
pip install esptool

# if that does not work:

git clone https://github.com/espressif/esptool.git ~/esptool
cd ~/esptool
python setup.py install

# on MacOs, Homebrew also works
brew install esptool
```

Then, from a terminal, just run the `make` commands from your project's folder.

> You don't really need "make" installed in order to build.
> Just look at the commands inside the example Makefile to build without `make`

```
make build

// you will see something like this:

CC project/src/index.c
AR project/build/esp8266_app.a
LD project/build/esp8266.out
FW project/firmware/
esptool.py v1.2

// then flash your rom to the defaults
make flash

// or specify a serial port to flash
ESP_PORT=/dev/tty.usbserialport make flash

```
