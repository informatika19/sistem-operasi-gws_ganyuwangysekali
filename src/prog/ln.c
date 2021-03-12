#include "progs.h"
#include "parse.h"
#include "stds.h"
#include "file.h"

void ln(char *path, char *outputPath, char parentIndex)
{
	int errno;
	char *content;
	readFile(content, path, &errno, parentIndex);
	if(errno == -1) // file tidak ditemukan
	{
		printString("No such file or directory");
		return;
	}
	if(errno == -2) // adalah sebuah directory
	{
		printString("Is a directory");
		return;
	}
	
	// file inputnya ada dan valid
	char inputFileIdx = getPathIndex(path, parentIndex);
	
	char inputFilename[14];
	getFilename(path, inputFilename);
	
	// semi write file :)
	char map[512], dir[1024], sect[512];
	char outputFilename[14], outputBasepath[512], outputParentIndex;
	
	int i = 0, j, k;
	unsigned char valid = 0;

  	getFilename(outputPath, outputFilename);
  	getBasePath(outputPath, outputBasepath, parentIndex);
  	outputParentIndex = getPathIndex(basepath, parentIndex);
  	
  	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	while(i < 0x40 && (dir[(i << 4) + 1] == 0xFF || dir[(i << 4) + 1] < 0x40)){
		if(dir[i << 4] == outputParentIndex && strncmp(dir+(i << 4) + 2, outputFilename, 14) != 0)
		{
			// file sudah ada :D
			errno = -1;
			return;
		}
		if(i == parentIndex) valid = 1;
		i++;
	}
	if(i > 0x3F){
		// tidak ada dir kosong
		errno = -2;
		return;
	}
	if(!valid)
	{
		// folder tidak valid
    		errno = -4;
    		return;
	}
	
	if(errno == -1)
	{
		printString("failed to create hard link: File exists");
		return;
	}
	else if(errno == -2)
	{
		printString("failed to create hard link: No empty space");
		return;
	}
	else if(errno == -4)
	{
		printString("Not a valid file or directory name");
		return;
	}
	
	dir[i << 4] = outputParentIndex;
	dir[i << 4 + 1] = dir[inputFileIdx << 4 + 1]; // menunjuk ke sektor yang sama
	// copy filename to buffer
	strncpy(dir + (i << 4) + 2, outputFilename, 14);
	
	writeSector(dir, 0x101);
	writeSector(dir + 512, 0x102);
}

// TODO : Implementation
void softln(char *path, char *outputPath, char parentIndex)
{
	return;
}

int main(int argc, char *argv[])
{
	if(argc == 3)
	{
		ln(argv[1], argv[2], cwdIdx);
	}
	else if(argc == 4)
	{
		softln(argv[2], argv[3], cwdIdx);
	}
}
