#include "progs.h"
#include "stds.h"
#include "string.h"
#include "buffer.h"
#include "file.h"

void ls(char *arg, char parentIndex)
{
	char dir[1024];
	char current[16];
	int i;
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);

	while(*arg == ' ') arg++;
	if(*arg != 0) parentIndex = getPathIndex(arg, parentIndex);

	if(parentIndex != 0xFF && dir[(parentIndex << 4) + 1] > 0x20 && dir[(parentIndex << 4)+1] != 0xFF)
	{
		parentIndex = dir[(parentIndex << 4) + 1];
	}
	
	for(i = 0; i <= 0x3F; i++)
	{
		// parent dari isifile = direktori sekarang
		if (dir[i<<4] == parentIndex && dir[(i<<4)+2] != 0)
		{
			clear(current, 16);
			strncpy(current, dir+(i<<4)+2, 14);
			current[strlen(current)] = '\n';
			printString(current);
		}
	}
}
/*
int main(int argc, char *argv[])
{
	char *content;
	ls(content, cwdIdx);
	printString(content);
}
*/