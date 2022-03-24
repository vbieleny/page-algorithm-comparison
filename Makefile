BUILDDIR        := build
USERBUILDDIR    := build/user
SRCDIR          := src
INCLUDEDIR      := include
TEMPLATEDIR     := .
CDCONTENTS      := $(BUILDDIR)/cdcontents
SRCS            := $(wildcard $(SRCDIR)/*.c)
USERSRCS        := $(wildcard ../$(SRCDIR)/*.c)
OBJS            := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCS:$(SRCDIR)/%=%))
OBJS            += $(patsubst %.c, $(USERBUILDDIR)/%.o, $(USERSRCS:../$(SRCDIR)/%=%))
DEPS            := $(OBJS:.o=.d)
LINKER          := $(SRCDIR)/linker.ld
ELFFILE         := os.elf
BINFILE         := os.bin
IMGFILE         := os.img
ISOFILE         := os.iso

ASM 	        := /usr/bin/nasm
CC  	        := /usr/local/cross/bin/i686-elf-gcc
OBJCOPY         := /usr/local/cross/bin/i686-elf-objcopy
OCFLAGS         := -O binary
ASMFLAGS        := -I $(INCLUDEDIR)/ -f bin
CFLAGS 	        := -g -std=gnu11 -Wall -ffreestanding -mgeneral-regs-only -masm=intel -m32 -I$(INCLUDEDIR)
LDFLAGS         := -g -T $(LINKER) -ffreestanding -nostdlib

CFGFILE         := config.mk

.EXTRA_PREREQS := Makefile $(CFGFILE)
.PHONY: all template qemud qemus qemuk qemu bochs view clean
.SILENT: qemus

-include $(CFGFILE)

CFLAGS += -DIDENTITY_PAGES_COUNT=$(CFG_IDENTITY_PAGES)

all: $(BUILDDIR)/$(IMGFILE)

$(BUILDDIR)/$(ISOFILE): $(BUILDDIR)/$(IMGFILE)
	mkdir $(CDCONTENTS)
	cp $< $(CDCONTENTS)/
	mkisofs -o $@ -input-charset utf-8 -V PRAC -b $(notdir $<) $(CDCONTENTS)
	rm -rf $(CDCONTENTS)

$(BUILDDIR)/$(IMGFILE): $(BUILDDIR)/boot.bin $(BUILDDIR)/$(BINFILE)
	cat $^ > $@
	truncate -c -s 1440K $@

$(BUILDDIR)/$(BINFILE): $(BUILDDIR)/$(ELFFILE)
	$(OBJCOPY) $(OCFLAGS) $(BUILDDIR)/$(ELFFILE) $(BUILDDIR)/$(BINFILE)

$(BUILDDIR)/$(ELFFILE): $(LINKER) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

-include $(DEPS)

$(USERBUILDDIR)/%.o: ../$(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/boot.bin: $(SRCDIR)/boot.asm | $(BUILDDIR)
	$(ASM) $(ASMFLAGS) -o $@ $<

$(BUILDDIR):
	mkdir -p $(sort $(dir $(OBJS)))

template:
	rm -rf $(TEMPLATEDIR)/.template
	mkdir -p $(TEMPLATEDIR)/.template/.prac
	cp -r src include Makefile bochsrc.txt $(TEMPLATEDIR)/.template/.prac/
	cp -r suite $(TEMPLATEDIR)/.template/src
	cp config.mk $(TEMPLATEDIR)/.template/
	sed -i 's+CFGFILE\s*:=\s*config.mk+CFGFILE := ../config.mk+g' $(TEMPLATEDIR)/.template/.prac/Makefile

qemud: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -s -S -icount 0 -serial stdio -drive file=$<,format=raw,index=0,media=disk

qemus: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -icount 0 -nographic -drive file=$<,format=raw,index=0,media=disk

qemuk: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -enable-kvm -serial stdio -drive file=$<,format=raw,index=0,media=disk

qemu: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -icount 0 -serial stdio -drive file=$<,format=raw,index=0,media=disk

bochs: $(BUILDDIR)/$(IMGFILE)
	bochs -f bochsrc.txt -q

clean:
	-rm -rf qemuout.txt bochsout.txt bx_enh_dbg.ini .template $(BUILDDIR)
