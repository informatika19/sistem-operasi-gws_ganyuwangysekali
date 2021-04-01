#include "progs.h"
#include "stds.h"
#include "file.h"

// parentIndex disimpan di cwdIdx
char chdir(char* inputPath, int* result, char parentIndex)
{
	char pathIndex, dir[1024];
	while(*inputPath == ' ') inputPath++;
	if(*inputPath == 0)
	{
		*result = 0;
		return 0xFF;
	}
	
	pathIndex = getPathIndex(inputPath, parentIndex);

	if(pathIndex == 0xFE)
	{
		*result = 2;
		return parentIndex;
	}

	if(pathIndex == 0xFF){
		*result = 0;
		return 0xFF;
	}
	
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	// kasus softlink
	if(dir[(pathIndex<<4)+1] > 0x1F && dir[(pathIndex<<4)+1] != 0xFF) pathIndex = dir[(pathIndex << 4) + 1] - 0x20;
	
	// sectornya itu sector file
	if ((dir[(pathIndex << 4) + 1] <= 0x1F) && (dir[(pathIndex << 4) + 1] >= 0x00))
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
	cwd = chdir(argv[1], &errno, cwd;
	
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