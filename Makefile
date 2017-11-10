# Project Config
PROJ_NAME=KoppenClima
ADDR_1=0x00000
ADDR_2=0x20000
COMPORT=/dev/ttyUSB0

# Tools settup
CC=$(XTENSA_GCC_DIR)/xtensa-lx106-elf-gcc
SIZE=$(XTENSA_GCC_DIR)/xtensa-lx106-elf-size
ESPTOOL_CK=$(ESPTOOL_CK_DIR)/esptool

# Include paths
IPATH= 		-I$(ESP8266_SDK_ROOT) \
					-I$(ESP8266_SDK_ROOT)/espressif \
					-I$(ESP8266_SDK_ROOT)/FreeRTOS/include \
					-I$(ESP8266_SDK_ROOT)/FreeRTOS/portable/esp8266 \
					-I$(ESP8266_SDK_ROOT)/cJSON \
					-I$(ESP8266_SDK_ROOT)/lwip/ \
					-I$(ESP8266_SDK_ROOT)/lwip/ipv4 \
					-I$(ESP8266_SDK_ROOT)/lwip/ipv6 \
					-I./source

# Library paths
LDPATH=		-L$(ESP8266_SDK_ROOT)/lib \
					-L$(ESP8266_SDK_ROOT)/ld 				

# C compiler flags
CCFLAGS=	-Wimplicit-function-declaration \
					-mlongcalls \
					-ffunction-sections \
					-fdata-sections \
					-mtext-section-literals \
					-mno-serialize-volatile \
					-Os \
					-std=c99 \
					-D__ETS__ \
					-DICACHE_FLASH \
					-DXTENSA \
					-DUSE_US_TIMER

# Libs and Linker flags
LDFLAGS=	-nostdlib \
					-T$(ESP8266_SDK_ROOT)/ld/eagle.app.v6.ld \
					-Wl,--no-check-sections \
					-u call_user_start \
					-Wl,-static \
					-Wl,--start-group \
					-lgcc \
					-lhal \
					-lphy \
					-lpp \
					-lnet80211 \
					-lwpa \
					-lcrypto \
					-lmain \
					-lfreertos \
					-llwip \
					-lsmartconfig \
					-lespconn \
					-lcirom \
					-lmirom \
					-ljson \
					-Wl,--end-group

# Output directory
OUT_DIR=build

SOURCES:= $(shell find $(SOURCEDIR) -name '*.c')
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
OBJOUT= $(addprefix $(OUT_DIR)/,$(patsubst %.c, %.o, $(SOURCES)))

all: outdir bin

# Removed .iram0.text section
bin: $(PROJ_NAME).elf
	@echo "Generating Binaries"
	@$(ESPTOOL_CK) -bz 4M -eo $(OUT_DIR)/$< \
	-bo $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_1).bin  -bs .text -bs .data -bs .rodata \
	-bc -ec -eo $(OUT_DIR)/$< -es .irom0.text \
	$(OUT_DIR)/$(PROJ_NAME)_$(ADDR_2).bin -ec || true

$(PROJ_NAME).elf: $(OBJECTS)
	@echo "CC $@"
	@$(CC) $(CCFLAGS) $(IPATH) $(LDPATH)  $(OBJOUT) -o $(OUT_DIR)/$@  $(LDFLAGS)	
	@$(SIZE) $(OUT_DIR)/$@
	

$(OBJECTS): %.o : %.c
	@echo "CC $@"
	@mkdir -p $(OUT_DIR)/$(@D)
	@$(CC) $(CCFLAGS) $(IPATH) $(LDPATH) $(LDFLAGS) -c $<  -o $(OUT_DIR)/$@
	
outdir:
	@mkdir -p $(OUT_DIR)
    
clean:
	rm -f $(OUT_DIR)/$(PROJ_NAME).elf $(OUT_DIR)/*.o $(OUT_DIR)/*.bin
	
flash: all
	@echo "Flashing ..."
	@$(ESPTOOL_CK) -cp $(COMPORT) -bm qio -bz 4M -bf 40 -cd none -cb 115200 -ca $(ADDR_1) -cf $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_1).bin -ca $(ADDR_2) -cf $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_2).bin
#	esptool.py -p /dev/ttyUSB0 --baud 115200 write_flash -fs 32m -fm qio -ff 40m 0x00000 $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_1).bin 0x20000 $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_2).bin
	@echo "Finished"	
	
#		esptool.py -p /dev/ttyUSB0 --baud 115200 write_flash -fs 32m -fm qio -ff 40m 0x00000 $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_1).bin 0x20000 $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_2).bin
	
#@$(ESPTOOL_CK) -cp $(COMPORT) -bm qio -bz 4M -bf 40 -cd none -cb 115200 -ca $(ADDR_1) -cf $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_1).bin -ca $(ADDR_2) -cf $(OUT_DIR)/$(PROJ_NAME)_$(ADDR_2).bin
	