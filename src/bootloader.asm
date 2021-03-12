;bootload.asm
;Michael Black, 2007
;Modified by Asisten Sister, 2021

;This is a simple bootloader that loads and executes a kernel at sector 1

	bits 16
KSEG	equ	0x1000		;Lokasi kernel = 0x10000
KSIZE	equ	60		;Ukuran kernel = 10 sektor
KSTART	equ	1		;Lokasi kernel = sektor 1

	;boot loader starts at 0 in segment 0x7c00
	org 0h

	;let's put the kernel at KSEG:0
	;set up the segment registers
	mov ax,KSEG
	mov ds,ax
	mov ss,ax
	mov es,ax
	;let's have the stack start at KSEG:fff0
	mov ax,0xfff0
	mov sp,ax
	mov bp,ax

	;read in the kernel from the disk

        mov     cl,KSTART+1      ;cl holds sector number
        mov     dh,0     ;dh holds head number - 0
        mov     ch,0     ;ch holds track number - 0
        mov     ah,2            ;absolute disk read
        mov     al,KSIZE        ;read KSIZE sectors
        mov     dl,0            ;read from floppy disk A
        mov     bx,0		;read into 0 (in the segment)
        int     13h	;call BIOS disk read function

	;call the kernel
	jmp KSEG:0

	times 510-($-$$) db 0

	;AA55 tells BIOS that this is a valid bootloader
	dw 0xAA55
