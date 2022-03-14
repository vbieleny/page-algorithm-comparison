BUILD_DIR  := build
SRC_DIR    := src
CDCONTENTS := $(BUILD_DIR)/cdcontents
KERNELINC  := $(SRC_DIR)/kernel/include
LIBCINC    := $(SRC_DIR)/libc/include
MODULES    := kernel libc suite
SOURCES    := $(wildcard $(addprefix src/, $(addsuffix /*.c, $(MODULES))))
OBJECTS    := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES:$(SRC_DIR)/%=%))
OBJDIRS    := $(patsubst %, $(BUILD_DIR)/%, $(MODULES))
DEPS       := $(OBJECTS:.o=.d)
LINKER     := $(SRC_DIR)/linker.ld
ELFFILE    := os.elf
BINFILE    := os.bin
IMGFILE    := os.img
ISOFILE    := os.iso

ASM 	   := /usr/bin/nasm
CC  	   := /usr/local/cross/bin/i686-elf-gcc
OBJCOPY    := /usr/local/cross/bin/i686-elf-objcopy
OCFLAGS    := -O binary
ASMFLAGS   := -I $(SRC_DIR)/boot/include/ -f bin
CFLAGS 	   := -g -std=gnu11 -Wall -ffreestanding -mgeneral-regs-only -masm=intel -I$(KERNELINC) -I$(LIBCINC)
LFLAGS     := -g -T $(LINKER) -ffreestanding -nostdlib

.PHONY: all qemud qemu bochs view clean

all: $(BUILD_DIR)/$(IMGFILE)

$(BUILD_DIR)/$(ISOFILE): $(BUILD_DIR)/$(IMGFILE)
	mkdir $(CDCONTENTS)
	cp $< $(CDCONTENTS)/
	mkisofs -o $@ -input-charset utf-8 -V PRAC -b $(notdir $<) $(CDCONTENTS)
	rm -rf $(CDCONTENTS)

$(BUILD_DIR)/$(IMGFILE): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/$(BINFILE)
	cat $^ > $@
	truncate -c -s 1440K $@

$(BUILD_DIR)/$(BINFILE): $(BUILD_DIR)/$(ELFFILE)
	$(OBJCOPY) $(OCFLAGS) $(BUILD_DIR)/$(ELFFILE) $(BUILD_DIR)/$(BINFILE)

$(BUILD_DIR)/$(ELFFILE): $(LINKER) $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir -p $(dir $(OBJECTS))

qemud: $(BUILD_DIR)/$(IMGFILE)
	qemu-system-i386 -s -S -drive file=$<,format=raw,index=0,media=disk

qemu: $(BUILD_DIR)/$(IMGFILE)
	qemu-system-i386 -drive file=$<,format=raw,index=0,media=disk

bochs: $(BUILD_DIR)/$(IMGFILE)
	bochs -f bochsrc.txt -q

view: $(BUILD_DIR)/$(BINFILE)
	ghex $<

clean:
	rm -rf qemuout.txt bochsout.txt bx_enh_dbg.ini $(BUILD_DIR)
