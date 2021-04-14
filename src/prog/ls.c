#include "string.h"
#include "fileio.h"
#include "basicio.h"

void ls(char *arg, char parentIndex)
{
	char dir[1024];
	char current[16];
	int i;
	lib_readSector(dir, 0x101);
	lib_readSector(dir + 512, 0x102);

	while(*arg == ' ') arg++;
	if(*arg != 0) parentIndex = getParent(arg, parentIndex);

	if(parentIndex != 0xFF && dir[(parentIndex << 4) + 1] > 0x20 && dir[(parentIndex << 4)+1] != 0xFF)
	{
		parentIndex = dir[(parentIndex << 4) + 1];
	}
	
	for(i = 0; i < 0x40; i++)
	{
		// parent dari isifile = direktori sekarang
		if (dir[i << 4] == parentIndex && dir[(i << 4) + 2] != 0)
		{
			clear(current, 16);
			strncpy(current, dir+(i << 4) + 2, 14);
			current[strlen(current)] = '\n';
			print(current);
		}
	}
}

int main()
{

}