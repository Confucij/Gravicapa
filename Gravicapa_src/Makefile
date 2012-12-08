APP=Gravicapa


CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJSIZE=arm-none-eabi-size


#Debug mode
#DEBUG=-g
#Release mode
DEBUG= -g

STDPL_FLAGS= -DHSE_VALUE=8000000 -DSTM32F10X_MD_VL

CCFLAGS=$(DEBUG) $(STDPL_FLAGS) -mlittle-endian -mthumb -ffreestanding -std=gnu99 # -ffunction-sections -fdata-sections


INCLUDE=-I./libs_stm/inc/base 
INCLUDE+=-I./libs_stm/inc/core_support 
INCLUDE+=-I./libs_stm/inc/device_support 
INCLUDE+=-I./include 
INCLUDE+=-I./libs_stm/inc/stm32f10x 
INCLUDE+=-I./include/sd
INCLUDE+=-I./include/onewire
INCLUDE+=-I./include/os 
INCLUDE+=-I./include/uIP

LDSCRIPT=stm32f100xb_flash.ld
LDFLAGS=-L./startup_files
LDFLAGS+=-Wl,-T,$(LDSCRIPT)
#LDFLAGS+=-Wl,--gc-sections

CCFLAGS+=-mcpu=cortex-m3

SRCDIR=./src
SRCDIR+=./src/os
SRCDIR+=./src/sd
#SRCDIR+=./src/onewire
SRCDIR+=./src/hal
#SRCDIR+=./src/uIP
SRC=./startup_files/cm3_genstartup.c 
SRC+=$(wildcard $(addsuffix /*.c, $(SRCDIR)))

all: $(APP).bin 

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

$(APP).elf: $(SRC) 
	$(CC) $(CCFLAGS) $(INCLUDE) $(LDFLAGS) $^ -o $@

clean:
	rm -rf *.elf
	rm -rf *.bin

.PHONY: all clean
