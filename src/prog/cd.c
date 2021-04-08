#include "progs.h"
#include "stds.h"
#include "file.h"

char chdir(int argc, char* args[], int* result, char parentIndex)
{
	// cd
	if(argc == 1)
	{
		*result = 0;
		return 0xFF;
	}

	char pathIndex, files[1024];
	
	pathIndex = getPathIndex(args[1], parentIndex);

	if(pathIndex == 0xFE)
	{
		*result = 2;
		return parentIndex;
	}

	if(pathIndex == 0xFF){
		*result = 0;
		return 0xFF;
	}
	
	readSector(files, 0x101);
	readSector(files + 512, 0x102);
	
	// kasus softlink
	while (files[(pathIndex << 4) + 1] >= 0x20 && files[(pathIndex << 4) + 1] != 0xFF)
	{
		pathIndex = files[(pathIndex << 4) + 1] - 0x20;
	}
	
	// sectornya itu sector file
	if ((files[(pathIndex << 4) + 1] <= 0x1F) && (files[(pathIndex << 4) + 1] >= 0x00))
	{
		*result = 1;
		return parentIndex;
	}
	
	*result = 0;
	return pathIndex;
}

/*
int main(int argc, char *argv[])
{
	int errno = 0;
	cwd = chdir(argc, argv, &errno, cwd);
	
	if(errno == 1)
	{
		printString("Not a directory");
	}
	else if(errno == 2)
	{
		printString("No such file or directory");
	}
	return errno;
}
*/