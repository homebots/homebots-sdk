FROM darlanalves/espbuild:latest

CMD make clean && make && cp esp-open-sdk/sdk/bin/esp_init_data_default.bin project/firmware/0x7c000.bin

ADD extras /home/espbuilder/extras
COPY Makefile /home/espbuilder/
