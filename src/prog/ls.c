#include "progs.h"
#include "stds.h"
#include "string.h"
#include "buffer.h"

void ls(char parentIndex)
{
	char dir[1024];
	char current[16];
	int i;
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	// if(dir[parentIndex << 4 + 1] > 0x20)
	// {
	// 	ls(dir[parentIndex << 4 + 1] - 0x20);
	// 	return;
	// }
	
	for(i = 0; i <= 0x3F; i++)
	{
		// parent dari isifile = direktori sekarang
		if (dir[16 * i] == parentIndex)
		{
			clear(current, 16);
			strncpy(current, dir+(i<<4)+2, 14);
			current[strlen(current)] = '\n';
			printString(current);
		}
	}
}

// int main(int argc, char *argv[])
// {
	// char *content;
	// ls(content, cwdIdx);
	// printString(content);
// }
