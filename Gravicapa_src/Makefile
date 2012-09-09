APP=Gravicapa


CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJSIZE=arm-none-eabi-size


#Debug mode
DEBUG=-g
#Release mode
#DEBUG=-s

CCFLAGS=$(DEBUG) -O2 -mlittle-endian -mthumb -ffreestanding -nostdlib -nostdinc


INCLUDE=-I./libs_stm/inc/base 
INCLUDE+=-I./libs_stm/inc/core_support 
INCLUDE+=-I./libs_stm/inc/device_support 
INCLUDE+=-I./include 

LDSCRIPT=stm32f100xb_flash.ld
LDFLAGS=-L./startup_files
LDFLAGS+=-Wl,-T,$(LDSCRIPT)

CCFLAGS+=-mcpu=cortex-m3  -DSTM32F10X_MD_VL

SRCDIR=./src 
#SRCDIR+=./libs_stm/src
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
