BUILDDIR        := build
USERBUILDDIR    := build/user
SRCDIR          := src
INCLUDEDIR      := include
TEMPLATEDIR     := .template
COMPILERFLAGS	:= $(BUILDDIR)/compilerflags
CDCONTENTS      := $(BUILDDIR)/cdcontents
SRCS            := $(wildcard $(SRCDIR)/*.c)
USERSRCS        := $(wildcard ../$(SRCDIR)/*.c)
OBJS            := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRCS:$(SRCDIR)/%=%))
OBJS            += $(patsubst %.c, $(USERBUILDDIR)/%.o, $(USERSRCS:../$(SRCDIR)/%=%))
OBJS			+= $(BUILDDIR)/pra_interrupts.o
DEPS            := $(OBJS:.o=.d)
LINKER          := $(SRCDIR)/pra_linker.ld
ELFFILE         := os.elf
BINFILE         := os.bin
IMGFILE         := os.img
ISOFILE         := os.iso

ASM 	        := /usr/bin/nasm
CC  	        := ${HOME}/.prac/gcc-i686-elf-toolchain/bin/i686-elf-gcc
OBJCOPY         := ${HOME}/.prac/gcc-i686-elf-toolchain/bin/i686-elf-objcopy
OCFLAGS         := -O binary
ASMFLAGS        := -I $(INCLUDEDIR)/ -f bin
ASMELFFLAGS     := -I $(INCLUDEDIR)/ -f elf
CFLAGS 	        := -g -std=gnu11 -Wall -ffreestanding -masm=intel -m32 -I$(INCLUDEDIR)
LDFLAGS         := -g -T $(LINKER) -ffreestanding -nostdlib
LIBS			:= -static-libgcc -lgcc

CFGFILE         := prac.ini

.EXTRA_PREREQS := Makefile $(CFGFILE)
.PHONY: all docs template qemu-debug qemu-terminal qemu-serial qemu-file bochs bochs-iso test test-build clean force

-include $(CFGFILE)

CFLAGS += -DIDENTITY_PAGES_COUNT=$(CFG_IDENTITY_PAGES)

all: $(BUILDDIR)/$(IMGFILE)

$(BUILDDIR)/$(ISOFILE): CFLAGS += -DDEFAULT_STREAM=IO_TERMINAL -DDEFAULT_FORMAT=FORMAT_HUMAN_READABLE
$(BUILDDIR)/$(ISOFILE): $(BUILDDIR)/$(IMGFILE)
	mkdir $(CDCONTENTS)
	cp $< $(CDCONTENTS)/
	mkisofs -o $@ -input-charset utf-8 -V PRAC -b $(notdir $<) $(CDCONTENTS)
	rm -rf $(CDCONTENTS)

$(BUILDDIR)/$(IMGFILE): $(BUILDDIR)/pra_boot.bin $(BUILDDIR)/$(BINFILE)
	cat $^ > $@
	truncate -c -s 1440K $@

$(BUILDDIR)/$(BINFILE): $(BUILDDIR)/$(ELFFILE)
	$(OBJCOPY) $(OCFLAGS) $(BUILDDIR)/$(ELFFILE) $(BUILDDIR)/$(BINFILE)

$(BUILDDIR)/$(ELFFILE): $(LINKER) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

-include $(DEPS)

$(USERBUILDDIR)/%.o: ../$(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/pra_boot.bin: $(SRCDIR)/pra_boot.asm | $(BUILDDIR)
	$(ASM) $(ASMFLAGS) -o $@ $<

$(BUILDDIR)/pra_interrupts.o: $(SRCDIR)/pra_interrupts.asm | $(BUILDDIR)
	$(ASM) $(ASMELFFLAGS) -o $@ $<

$(BUILDDIR):
	mkdir -p $(sort $(dir $(OBJS)))

$(COMPILERFLAGS): force
	echo '$(CFLAGS)' | md5sum - | cut -f1 -d' ' | cmp -s - $@ || echo '$(CFLAGS)' | md5sum - | cut -f1 -d' ' > $@

$(OBJS): $(COMPILERFLAGS)

docs: Doxyfile
	doxygen $<

template:
	mkdir -p $(TEMPLATEDIR)/.prac
	cp -r src include Makefile bochsrc.txt bochsrc-iso.txt $(TEMPLATEDIR)/.prac/
	cp -r suite $(TEMPLATEDIR)/src
	cp prac.ini $(TEMPLATEDIR)/
	sed -i 's+CFGFILE\s*:=\s*prac.ini+CFGFILE := ../prac.ini+g' $(TEMPLATEDIR)/.prac/Makefile

qemu-debug: CFLAGS += -DDEFAULT_STREAM=IO_TERMINAL -DDEFAULT_FORMAT=FORMAT_HUMAN_READABLE
qemu-debug: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -s -S -icount 0 -serial stdio -drive file=$<,format=raw,index=0,media=disk

qemu-terminal: CFLAGS += -DDEFAULT_STREAM=IO_TERMINAL -DDEFAULT_FORMAT=FORMAT_HUMAN_READABLE
qemu-terminal: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -icount 0 -drive file=$<,format=raw,index=0,media=disk

qemu-serial: CFLAGS += -DDEFAULT_STREAM=IO_SERIAL -DDEFAULT_FORMAT=FORMAT_HUMAN_READABLE
qemu-serial: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -icount 0 -display none -serial stdio -drive file=$<,format=raw,index=0,media=disk

qemu-file: CFLAGS += -DDEFAULT_STREAM=IO_SERIAL -DDEFAULT_FORMAT=FORMAT_PARSEABLE
qemu-file: $(BUILDDIR)/$(IMGFILE)
	qemu-system-i386 -icount 0 -display none -serial file:prac-output.out -drive file=$<,format=raw,index=0,media=disk

bochs: CFLAGS += -DDEFAULT_STREAM=IO_TERMINAL -DDEFAULT_FORMAT=FORMAT_HUMAN_READABLE
bochs: $(BUILDDIR)/$(IMGFILE)
	bochs -f bochsrc.txt -q

bochs-iso: $(BUILDDIR)/$(ISOFILE)
	bochs -f bochsrc-iso.txt -q

test:
	$(MAKE) -C test $@

test-build:
	$(MAKE) -C test all

clean:
	-rm -rf qemuout.txt bochsout.txt bochsout-iso.txt bx_enh_dbg.ini .template docs/html $(BUILDDIR)
	$(MAKE) -C test $@
