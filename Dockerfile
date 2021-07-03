FROM darlanalves/espbuild:latest

RUN cd esp-open-sdk
CMD make clean && make && cp esp-open-sdk/sdk/bin/esp_init_data_default.bin project/firmware/0x7c000.bin

ADD sdk /home/espbuilder/sdk
COPY Makefile /home/espbuilder/
