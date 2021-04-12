#include "stds.h"
#include "file.h"
#include "shell.h"

void ln(char* args, char parentIndex)
{
	int errno;
	char content[8192], dir[1024], inputFileIdx, inputPath[128], outputPath[128], outputFilename[14], outputBasepath[512], outputParentIndex;
	unsigned char valid, isSoft;

	isSoft = 0;
	clear(inputPath, 128);
	clear(outputPath, 128);
	while(*args == ' ') args++;
	valid = 0;
	while(*args != 0){
		errno = 0;
		clear(dir, 128);
		while(*(args+errno) != ' ' && *(args+errno) != 0) errno++;
		strncpy(dir, args, errno);
		if(strncmp(dir, "-s", 2) == 0) isSoft = 1;
		else if(strlen(inputPath) == 0) strncpy(inputPath, dir, 14);
		else if(strlen(outputPath) == 0) strncpy(outputPath, dir, 14);
		else{
			errno = -1;
			break;
		}
		valid++;
		args += errno+1;
	}
	if((valid > 3) || (errno == -1) || (valid < 2)){
		printString("Usage: ln [-s] <input> <output>\n");
		return;
	}

	/* Mengolah path input */
	readFile(content, inputPath, &errno, parentIndex);
	if(errno == -1) // file tidak ditemukan
	{
		printString("No such file or directory\n");
		return;
	}

	if(isSoft==0 && errno == -2){
		printString("Hard link not allowed for directory\n");
		return;
	}
	/* Mengolah path output */
	getFilename(outputPath, outputFilename);
	getBasePath(outputPath, outputBasepath, parentIndex);
	readFile(content, outputFilename, &errno, parentIndex);
	if(errno == 0 || errno == -2) // file outputnya sudah ada :v
	{
		printString("failed to create hard link: File exists\n");
		return;
	}

	/* DI BAGIAN INI, FILE INPUT MAUPUN OUTPUT SUDAH VALID */
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	inputFileIdx = getPathIndex(inputPath, parentIndex);
	
	outputParentIndex = getPathIndex(outputBasepath, parentIndex);
	errno = 0;
	while(errno <= 0x3F)
	{
		// dengan asumsi namafile tidak kosong
		if(dir[(errno << 4) + 2] == 0) break;
		errno++;
	}
	if(errno > 0x3F)
	{
		// tidak ada dir kosong
		printString("failed to create hard link: No empty space\n");
		return;
	}
	dir[errno << 4] = outputParentIndex;
	if(isSoft){
		// softlink
		dir[(errno<<4)+1] = 0x20+inputFileIdx;
	}
	else{
		// hardlink
		dir[(errno << 4) + 1] = dir[(inputFileIdx << 4) + 1]; // menunjuk ke sektor yang sama
	}
	// copy filename to buffer
	strncpy(dir + (errno << 4) + 2, outputFilename, 14);
	
	writeSector(dir, 0x101);
	writeSector(dir + 512, 0x102);
	return;
}

int main(){
}

/*
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
	
	// Mengolah path output //
	readFile(content, outputPath, &errno, parentIndex);
	if(errno == -1 || errno == -2) // file outputnya sudah ada
	{
		printString("failed to create soft link: File exists");
		return;
	}
	// DI BAGIAN INI, FILE INPUT MAUPUN OUTPUT SUDAH VALID //
	char dir[1024];
	readSector(dir, 0x101);
	readSector(dir + 512, 0x102);
	
	char inputFileIdx = getPathIndex(inputPath, parentIndex);
	
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
	dir[i << 4 + 1] = inputFileIdx + 0x40;
		
	// copy filename to buffer
	strncpy(dir + (i << 4) + 2, outputFilename, 14);
	
	writeSector(dir, 0x101);
	writeSector(dir + 512, 0x102);
	return;
}
*/

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
