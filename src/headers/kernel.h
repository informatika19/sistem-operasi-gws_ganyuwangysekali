#ifndef _KERNEL_H
#define _KERNEL_H

#define bufsize 20
#define textSize 1000

#define VGA_MEMORY_BASE 0xA000
#define VGA_WIDTH 320
#define VGA_HEIGHT 200

#define TEXT_MEMORY_BASE 0xB000
#define TEXT_OFFSET 0x8000

char map[512];
char files[1024];
char sector[512];

void printString(char *string);
void readString(char *string);
void clearScreen();
void clear(char *buffer, int length);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
#endif
