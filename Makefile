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

$(sys_img): $(out) $(bootloader) $(kernel)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(bootloader) of=$@ bs=512 conv=notrunc count=1
	dd if=$(kernel) of=$@ bs=512 conv=notrunc seek=1

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
