#include "progs.h"
#include "stds.h"

void ls(char *content, char parentIndex)
{
	char dir[1024];
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	while(dir[parentIndex << 4 + 1] > 0x40)
	{
		parentIndex = dir[parentIndex << 4 + 1] - 0x40;
	}
	
	for(int i = 0; i <= 0x3F; i++)
	{
		// parent dari isifile = direktori sekarang
		if (dir[16 * i] == parentIndex)
		{
			strncat(content, dir + (i << 4) + 2, 14);
			strncat(content, "\n", 1);
		}
	}
	strncat(content, 0, 1);
	return;
}

/*
int main(int argc, char *argv[])
{
	char *content;
	ls(content, cwdIdx);
	printString(content);
}
*/
