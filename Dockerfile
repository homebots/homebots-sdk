FROM darlanalves/espbuild:latest

RUN cd esp-open-sdk && cp ESP8266_NONOS_SDK-2.1.0-18-g61248df/lib/libgcc.a xtensa-lx106-elf/lib/gcc/xtensa-lx106-elf/4.8.5/
CMD make clean && make && cp esp-open-sdk/sdk/bin/esp_init_data_default.bin project/firmware/0x7c000.bin

ADD sdk /home/espbuilder/sdk
COPY Makefile /home/espbuilder/
