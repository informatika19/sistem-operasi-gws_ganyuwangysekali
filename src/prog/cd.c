#include "basicio.h"
#include "fileio.h"

char chdir(char* inputPath, char parentIndex);

int main()
{
	char fileBuf[8192], arg[512];
	int errNo = 0;
	char cwd, parent;
	
	lib_readFile(fileBuf, "shellcwd", &errNo, 0xFF);
    if(errNo == 1)
        cwd = fileBuf[0];
    else
        cwd = 0xFF;

	clear(fileBuf, 8192);

	lib_readFile(fileBuf, "tempc", &errNo, 0xFF);
	removeFEntry("tempc", 0xFF, &errNo);
	parse(fileBuf, &parent, arg);

	cwd = chdir(arg, cwd);
	removeFEntry("shellcwd", 0xFF, &errNo);

	clear(fileBuf, 8192);
	fileBuf[0] = cwd;
	errNo = 16;

	lib_writeFile(fileBuf, "shellcwd", &errNo, 0xFF);
}

char chdir(char* inputPath, char parentIndex)
{
	char pathIndex, dir[1024];
	while(*inputPath == ' ') inputPath++;
	if(*inputPath == 0)
	{
		return 0xFF;
	}
	
	pathIndex = getParent(inputPath, parentIndex);

	if(pathIndex == 0xFE)
	{
		print("No such file or directory\n");
		return parentIndex;
	}

	if(pathIndex == 0xFF){
		return 0xFF;
	}
	
	lib_readSector(dir, 0x101);
	lib_readSector(dir + 512, 0x102);
	
	// kasus softlink
	if(dir[(pathIndex << 4) + 1] > 0x1F && dir[(pathIndex << 4)+1] != 0xFF) pathIndex = dir[(pathIndex << 4) + 1] - 0x20;

	// sectornya itu sector file
	if ((dir[(pathIndex << 4) + 1] <= 0x1F) && (dir[(pathIndex << 4) + 1] >= 0x00))
	{
		print("Not a directory\n");
		return parentIndex;
	}
	return pathIndex;
}
