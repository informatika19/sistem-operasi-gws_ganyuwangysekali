#ifndef _KERNEL_H
#define _KERNEL_H

#define bufsize 50
#define textSize 1000

#define VGA_MEMORY_BASE 0xA000
#define VGA_WIDTH 320
#define VGA_HEIGHT 200

#define TEXT_MEMORY_BASE 0xB000
#define TEXT_OFFSET 0x8000

// Milestone 1
void printString(char *string);
void readString(char *string);
void clearScreen();

#endif