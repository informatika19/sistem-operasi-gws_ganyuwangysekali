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
		interrupt(0x21, 0, "Usage: ln [-s] <input> <output>\n", 0, 0);
		return;
	}

	/* Mengolah path input */
	// readFile(content, inputPath, &errno, parentIndex);
	interrupt(0x21, (parentIndex << 8) | 0x04, content, inputPath, &errno);
	if(errno == -1) // file tidak ditemukan
	{
		interrupt(0x21, 0, "No such file or directory\n", 0, 0);
		return;
	}

	if(isSoft==0 && errno == -2){
		interrupt(0x21, 0, "Hard link not allowed for directory\n", 0, 0);
		return;
	}
	/* Mengolah path output */
	getFilename(outputPath, outputFilename);
	getBasePath(outputPath, outputBasepath, parentIndex);
	//readFile(content, outputFilename, &errno, parentIndex);
	interrupt(0x21, (parentIndex << 8) | 0x04, content, outputFilename, &errno);
	if(errno == 0 || errno == -2) // file outputnya sudah ada :v
	{
		interrupt(0x21, 0, "failed to create hard link: File exists\n", 0, 0);
		return;
	}

	/* DI BAGIAN INI, FILE INPUT MAUPUN OUTPUT SUDAH VALID */
//	readSector(dir, 0x101);
//	readSector(dir + 512, 0x102);
	interrupt(0x21, 0x0002, dir, 0x101, 0);
	interrupt(0x21, 0x0002, dir + 512, 0x102);
	
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
		interrupt(0x21, 0, "failed to create hard link: No empty space\n", 0, 0);
		return;
	}
	dir[errno << 4] = outputParentIndex;
	if(isSoft){
		// softlink
		dir[(errno << 4) + 1] = 0x20 + inputFileIdx;
	}
	else{
		// hardlink
		dir[(errno << 4) + 1] = dir[(inputFileIdx << 4) + 1]; // menunjuk ke sektor yang sama
	}
	// copy filename to buffer
	strncpy(dir + (errno << 4) + 2, outputFilename, 14);
	
//	writeSector(dir, 0x101);
//	writeSector(dir + 512, 0x102);
	interrupt(0x21, 0x0003, dir, 0x101, 0);
	interrupt(0x21, 0x0003, dir + 512, 0x102, 0);
	return;
}

int main(){
}