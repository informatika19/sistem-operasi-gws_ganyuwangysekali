TOOLS=tools
BOCHS ?= bochs
SRC=src
CFLAGS=-ansi -c
kernel_c=$(SRC)/kernel.c
kernel_asm=$(SRC)/kernel.asm
bootloader_asm=$(SRC)/bootloader.asm
boot_logo_in=$(SRC)/boot_logo

out=out

incl=$(SRC)/headers/
INCL_FLAG=-I$(incl)

KSIZE=50

map_img=$(out)/map.img
files_img=$(out)/files.img
sectors_img=$(out)/sectors.img

sys_img=$(out)/system.img
bootloader=$(out)/bootloader
kernel_o=$(out)/kernel.o
kernel_asm_o=$(out)/kernel_asm.o
kernel=$(out)/kernel
boot_logo=$(out)/logo.bin

_IMPL=math string
IMPL=$(patsubst %, $(out)/impl_%.o, $(_IMPL))
_PROGS=cat cd ln ls
PROGS=$(patsubst %, $(out)/prog_%.o, $(_PROGS))

$(out)/impl_%.o: $(SRC)/impl/%.c
	bcc $(CFLAGS) $(INCL_FLAG) -o $@ $<

$(out)/prog_%.o: $(SRC)/prog/%.c
	bcc $(CFLAGS) $(INCL_FLAG) -o $@ $<

$(out):
	mkdir $(out)

$(boot_logo): $(boot_logo_in)
	python3 $(TOOLS)/image2bin.py $< $@

$(map_img):
	dd if=/dev/zero of=$@ bs=512 count=1
	# isi KSIZE sektor pertama dengan 0xFF, karena kernel sudah mengisi fs
	python -c 'print("\xFF"*$(KSIZE))' | dd of=$@ bs=512 count=$(KSIZE) conv=notrunc

$(sectors_img):
	dd if=/dev/zero of=$@ bs=512 count=1

$(files_img):
	dd if=/dev/zero of=$@ bs=512 count=2

$(sys_img): $(out) $(bootloader) $(kernel) $(map_img) $(sectors_img) $(files_img)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(bootloader) of=$@ bs=512 conv=notrunc count=1
	dd if=$(kernel) of=$@ bs=512 conv=notrunc seek=1
	dd if=$(map_img) of=$@ bs=512 count=1 seek=256
	dd if=$(files_img) of=$@ bs=512 count=2 seek=257
	dd if=$(sectors_img) of=$@ bs=512 count=1 seek=259

$(bootloader): $(bootloader_asm)
	nasm $< -o $@

$(kernel_o): $(kernel_c)
	bcc $(CFLAGS) $(INCL_FLAG) -o $@ $<

$(kernel_asm_o): $(kernel_asm) $(out) $(boot_logo)
	nasm -i $(out) -f as86 $< -o $@

$(kernel): $(kernel_o) $(IMPL) $(PROGS) $(kernel_asm_o)
	ld86 -o $@ -d $^

run: $(sys_img)
	$(BOCHS) -f if2230.config

clean:
	rm -rf out/*

.PHONY: build run clean
