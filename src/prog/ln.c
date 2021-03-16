#include "progs.h"
#include "stds.h"
#include "file.h"
#include "shell.h"

void ln(char *inputPath, char *outputPath, char parentIndex)
{
	int errno;
	char *content;
	
	/* Mengolah path input */
	readFile(content, inputPath, &errno, parentIndex);
	if(errno == -1) // file tidak ditemukan
	{
		printString("No such file or directory");
		return;
	}
	if(errno == -2) // adalah sebuah directory
	{
		printString("Hard link not allowed for directory");
		return;
	}
	
	/* Mengolah path output */
	readFile(content, outputPath, &errno, parentIndex);
	if(errno == -1 || errno == -2) // file outputnya sudah ada :v
	{
		printString("failed to create hard link: File exists");
		return;
	}
	/* DI BAGIAN INI, FILE INPUT MAUPUN OUTPUT SUDAH VALID */
	char dir[1024];
  	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	char inputFileIdx = getPathIndex(inputPath, parentIndex);
	char inputFilename[14];
	getFilename(inputPath, inputFilename);
	
	char outputFilename[14], outputBasepath[512], outputParentIndex;
  	getFilename(outputPath, outputFilename);
  	getBasePath(outputPath, outputBasepath, parentIndex);
  	outputParentIndex = getPathIndex(outputBasepath, parentIndex);
  	
  	int i = 0;
	unsigned char valid = 0;
	while(i <= 0x3F)
	{
		// dengan asumsi namafile tidak kosong
		if(dir[i << 4 + 2] == 0) break;
		i++;
	}
	
	if(i > 0x3F)
	{
		// tidak ada dir kosong
		printString("failed to create hard link: No empty space");
		return;
	}
	
	dir[i << 4] = outputParentIndex;
	dir[i << 4 + 1] = dir[inputFileIdx << 4 + 1]; // menunjuk ke sektor yang sama
	// copy filename to buffer
	strncpy(dir + (i << 4) + 2, outputFilename, 14);
	
	writeSector(dir, 0x101);
	writeSector(dir + 512, 0x102);
	return;
}

void softln(char *inputPath, char *outputPath, char parentIndex)
{
	int errno;
	char *content;
	readFile(content, inputPath, &errno, parentIndex);
	if(errno == -1) // file tidak ditemukan
	{
		printString("No such file or directory");
		return;
	}
	if(errno == -2) // adalah sebuah directory
	{
		/* Mengolah path output */
		readFile(content, outputPath, &errno, parentIndex);
		if(errno == -1 || errno == -2) // file outputnya sudah ada
		{
			printString("failed to create soft link: File exists");
			return;
		}
		/* DI BAGIAN INI, FILE INPUT MAUPUN OUTPUT SUDAH VALID */
		char dir[1024];
  		readSector(dir, 0x101);
		readSector(dir + 512, 0x102);
	
		char inputFileIdx = getPathIndex(inputPath, parentIndex);
		char inputFilename[14];
		getFilename(path, inputFilename);
		
		char outputFilename[14], outputBasepath[512], outputParentIndex;
  		getFilename(outputPath, outputFilename);
  		getBasePath(outputPath, outputBasepath, parentIndex);
  		outputParentIndex = getPathIndex(outputBasepath, parentIndex);
  	
  		int i = 0;
		unsigned char valid = 0;
		while(i <= 0x3F)
		{
			// dengan asumsi namafile tidak kosong
			if(dir[i << 4 + 2] == 0) break;
			i++;
		}
	
		if(i > 0x3F)
		{
			// tidak ada dir kosong
			printString("failed to create soft link: No empty space");
			return;
		}
	
		dir[i << 4] = outputParentIndex;
		dir[i << 4 + 1] = inputFileIdx + 0x20;
		
		// copy filename to buffer
		strncpy(dir + (i << 4) + 2, outputFilename, 14);
	
		writeSector(dir, 0x101);
		writeSector(dir + 512, 0x102);
		return;
	}
	
	// adalah sebuah file, hardlink aja :)
	ln(inputPath, outputPath, parentIndex);
	return;
}

/*
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
*/
