#include "stds.h"
#include "progs.h"
#include "string.h"
#include "buffer.h"
#include "kernel.h"


extern unsigned char logo[];

int main () {
	char buffer[bufsize];
	char sectorBuf[512]; // 1 sektor = 512 byte
	int x, y;
	int width = logo[0];
	int height = logo[1];
	int startx = (VGA_WIDTH - width)>>1;
	int starty = (VGA_HEIGHT - height)>>1;
	// set graphics mode
	// http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/rb-0069.htm
	// 13h = VGA Graphics (320x200, 256 colors)
	interrupt(0x10, 0x0013, 0x0000, 0x0000, 0x0000);
	// render graphics
	for(y = width; y > 0; y--){
		for(x = width; x > 0; x--){
			putInMemory(VGA_MEMORY_BASE, (y+starty) * VGA_WIDTH + (x+startx), logo[(y*width)+x]);
		}
	}

	makeInterrupt21();

	while(1){
		clear(buffer, bufsize);
		interrupt(0x21, 0x01, buffer, 0x0000, 0x0000);
		interrupt(0x21, 0x00, buffer, 0x0000, 0x0000);
	}
}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {
	char AL, AH;
	AL = (char) (AX);
	AH = (char) (AX >> 0);
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
//			readFile(BX, CX, DX, AH);
			break;
		case 0x05:
//			writeFile(BX, CX, DX, AH);
			break;
		default:
			printString("invalid interrupt");
	}
}

void printString(char *string){
	int i = 0;
	// set mode text 
	interrupt(0x10, 0x0003, 0x0000, 0x0000, 0x0000);
	clearScreen();
	for(i = 0; string[i] != 0; i++){
		// set posisi kursor
		interrupt(0x10, 0x0200, 0x0000, 0x0000, 0x0000+i);
		// tulis string :D
		interrupt(0x10, 0x0900 + string[i], 0x000D, 0x0001, 0x0000);
	}
}

void readString(char *string){
	int length = 0;
	char current = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	while(length < bufsize-1 && current != 0x0D){
		string[length] = current;
		length++;
		current = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	}
	string[length] = 0;
}

void clearScreen(){
	int x, y;
	for(x = VGA_WIDTH; x >= 0; x--){
		for(y = VGA_HEIGHT; y >= 0; y--){
			putInMemory(VGA_MEMORY_BASE, VGA_WIDTH*y + x, 0);
		}
	}
}