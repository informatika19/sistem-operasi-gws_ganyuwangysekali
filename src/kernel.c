#include "stds.h"
#include "progs.h"
#include "string.h"
#include "buffer.h"
#include "kernel.h"
#include "file.h"
#include "io.h"


void drawLogo();
int main ()
{
	// set graphics mode
	// http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/rb-0069.htm
	// 13h = VGA Graphics (320x200, 256 colors)
	// render graphics
	makeInterrupt21();

	drawLogo();

	printString("Press any key to continue...");
	interrupt(0x16, 0, 0, 0, 0);
	
	// set mode text 
	interrupt(0x10, 0x0003, 0x0000, 0x0000, 0x0000);
	interrupt(0x21, 0xFF06, "/bin/shell", 0x2000, 0);
	while(1);
}

void drawLogo(){
	int x, y;
	int width, height, startx, starty;
	char logo[512<<4];
	interrupt(0x21, 0xFF04, logo, "logo", &x);
	width = logo[0];
	height = logo[1]-1;
	startx = (VGA_WIDTH - width)>>1;
	starty = (VGA_HEIGHT - height)>>1;
	interrupt(0x10, 0x0013, 0x0000, 0x0000, 0x0000);
	for(y = height; y > 0; y--){
		for(x = width; x > 0; x--){
			putInMemory(VGA_MEMORY_BASE, (y+starty) * VGA_WIDTH + (x+startx), logo[(y*width)+x]);
		}
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
		case 0x6:
      executeProgram(BX, CX, DX, AH);
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

void executeProgram(char *filename, int segment, int *success, char parentIndex) {
    // Buat buffer
    int isSuccess;
    char fileBuffer[512 * 16];
		int i = 0;
    // Buka file dengan readFile
    readFile(fileBuffer, filename, &isSuccess, parentIndex);
    // If success, salin dengan putInMemory
    if (isSuccess == 1) {
        // launchProgram
        for (i = 0; i < 512*16; i++) {
            putInMemory(segment, i, fileBuffer[i]);
        }
        launchProgram(segment);
    } else {
        interrupt(0x21, 0, "File not found!", 0,0);
    }
}
