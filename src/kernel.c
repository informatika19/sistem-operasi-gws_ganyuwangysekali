#include "stds.h"
#include "progs.h"
#include "string.h"
#include "buffer.h"
#include "kernel.h"
#include "file.h"
#include "io.h"

extern unsigned char logo[];

int col = 0, row = 0;

int main ()
{
	char buffer[bufsize];
	char sectorBuf[512]; // 1 sektor = 512 byte
	int x, y;
	int width = logo[0];
	int height = logo[1]-1;
	int startx = (VGA_WIDTH - width)>>1;
	int starty = (VGA_HEIGHT - height)>>1;
	// set graphics mode
	// http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/rb-0069.htm
	// 13h = VGA Graphics (320x200, 256 colors)
	interrupt(0x10, 0x0013, 0x0000, 0x0000, 0x0000);
	// render graphics
	for(y = height; y > 0; y--){
		for(x = width; x > 0; x--){
			putInMemory(VGA_MEMORY_BASE, (y+starty) * VGA_WIDTH + (x+startx), logo[(y*width)+x]);
		}
	}

	makeInterrupt21();

	printString("Press any key to continue...");
	interrupt(0x16, 0, 0, 0, 0);
	
	runShell();
	// set mode text 
	interrupt(0x10, 0x0003, 0x0000, 0x0000, 0x0000);
	row = 0;
	col = 0;

	while(1){
		clear(buffer, bufsize);
		interrupt(0x21, 0x01, buffer, 0x0000, 0x0000);
	}
}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {
	char AL, AH;
	AL = (char) (AX);
	AH = (char) (AX >> 8);
	switch(AL)
	{	
		case 0x00:
			printString(BX);
			break;
		case 0x01:
			readString(BX);
			break;
		case 0x02:
			readSector(BX, CX);
			break;
		case 0x03:
			writeSector(BX, CX);
			break;
		case 0x04:
			readFile(BX, CX, DX, AH);
			break;
		case 0x05:
			writeFile(BX, CX, DX, AH);
			break;
		default:
			printString("invalid interrupt");
	}
}

void clearScreen(){
	int x, y;
	for(x = VGA_WIDTH; x >= 0; x--){
		for(y = VGA_HEIGHT; y >= 0; y--){
			putInMemory(VGA_MEMORY_BASE, VGA_WIDTH*y + x, 0);
		}
	}
}
