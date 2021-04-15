TOOLS=tools
BOCHS ?= bochs
SRC=src
CFLAGS=-ansi -c
kernel_c=$(SRC)/kernel.c
kernel_asm=$(SRC)/kernel.asm
bootloader_asm=$(SRC)/bootloader.asm
boot_logo_in=$(SRC)/boot_logo

out=out

libs=$(SRC)/lib/headers/
incl=$(SRC)/headers/ $(libs)
INCL_FLAG=$(patsubst %, -I%, $(incl))

KSIZE=16

map_img=$(out)/map.img
files_img=$(out)/files.img
sectors_img=$(out)/sectors.img

sys_img=$(out)/system.img
bootloader=$(out)/bootloader
kernel_o=$(out)/kernel.o
kernel_asm_o=$(out)/kernel_asm.o
kernel=$(out)/kernel
boot_logo=$(out)/logo.bin

IMPL_FOLDER=$(SRC)/impl
LIB_FOLDER=$(SRC)/lib/impl
PROGS_FOLDER=$(SRC)/prog

IMPL=$(patsubst $(IMPL_FOLDER)/%.c, $(out)/impl_%.o, $(wildcard $(IMPL_FOLDER)/*.c))
LIB=$(patsubst $(LIB_FOLDER)/%.c, $(out)/lib_%.o, $(wildcard $(LIB_FOLDER)/*.c))
PROGS=$(patsubst $(PROGS_FOLDER)/%.c, $(out)/prog_%, $(wildcard $(PROGS_FOLDER)/*.c))

all: $(sys_img)

$(sys_img): $(out) $(bootloader) $(kernel) $(map_img) $(sectors_img) $(files_img) $(boot_logo)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(bootloader) of=$@ bs=512 conv=notrunc count=1
	dd if=$(kernel) of=$@ bs=512 conv=notrunc seek=1
	dd if=$(map_img) of=$@ bs=512 count=1 seek=256 conv=notrunc
	dd if=$(files_img) of=$@ bs=512 count=2 seek=257 conv=notrunc
	dd if=$(sectors_img) of=$@ bs=512 count=1 seek=259 conv=notrunc
	python tools/loadfile.py $@ $(boot_logo) "logo"

$(out)/lib_%.o: $(LIB_FOLDER)/%.c
	bcc $(CFLAGS) $(INCL_FLAG) -o $@ $<

$(out)/impl_%.o: $(IMPL_FOLDER)/%.c
	bcc $(CFLAGS) $(INCL_FLAG) -o $@ $<

$(out)/prog_%.o: $(PROGS_FOLDER)/%.c
	bcc $(CFLAGS) -I$(libs) -o $@ $<

$(out)/lib_intr.o: $(SRC)/lib.asm
	nasm -f as86 $< -o $@

$(out)/prog_%: $(out)/prog_%.o $(LIB) $(out)/lib_intr.o
	ld86 -o $@ -d $^
	python tools/loadfile.py $(sys_img) $@ $(addprefix /bin/,$(patsubst $(out)/prog_%, %, $@))

$(out):
	mkdir $(out)

$(boot_logo): $(boot_logo_in)
	python3 $(TOOLS)/image2bin.py $< $@

$(map_img):
	dd if=/dev/zero of=$@ bs=512 count=1
	# isi KSIZE sektor pertama dengan 0xFF, karena kernel sudah mengisi fs
	python -c 'import sys; sys.stdout.buffer.write(b"\xFF"*$(KSIZE))' | dd of=$@ bs=512 count=1 conv=notrunc

$(sectors_img):
	dd if=/dev/zero of=$@ bs=512 count=1

$(files_img):
	dd if=/dev/zero of=$@ bs=512 count=2

$(bootloader): $(bootloader_asm)
	nasm $< -o $@

$(kernel_o): $(kernel_c)
	bcc $(CFLAGS) $(INCL_FLAG) -o $@ $<

$(kernel_asm_o): $(kernel_asm) $(out)
	nasm -i $(out) -f as86 $< -o $@

$(kernel): $(kernel_o) $(IMPL) $(LIB) $(kernel_asm_o)
	ld86 -o $@ -d $^

run: $(sys_img) #$(PROGS)
	$(BOCHS) -f if2230.config

clean:
	rm -rf out/*

.PHONY: build run clean all
