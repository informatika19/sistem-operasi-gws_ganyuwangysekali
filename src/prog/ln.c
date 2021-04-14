#include "string.h"
#include "fileio.h"
#include "basicio.h"

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
	while(*args != 0)
	{
		errno = 0;
		clear(dir, 128);
		while(*(args + errno) != ' ' && *(args + errno) != 0) errno++;
		strncpy(dir, args, errno);
		if(strncmp(dir, "-s", 2) == 0) isSoft = 1;
		else if(strlen(inputPath) == 0) strncpy(inputPath, dir, 128);
		else if(strlen(outputPath) == 0) strncpy(outputPath, dir, 128);
		else
		{
			errno = -1;
			break;
		}
		valid++;
		args += errno+1;
	}
	if((valid > 3) || (errno == -1) || (valid < 2))
	{
		print("Usage: ln [-s] <input> <output>\n");
		return;
	}

	/* Mengolah path input */
	// readFile(content, inputPath, &errno, parentIndex);
	lib_readFile(inputPath, parentIndex, content, &errno);
	if(errno == -1) // file tidak ditemukan
	{
		print("No such file or directory\n");
		return;
	}

	if(isSoft == 0 && errno == -2){
		print("Hard link not allowed for directory\n");
		return;
	}
	/* Mengolah path output */
	lib_getFileName(outputPath, outputFilename);
	lib_getAbsolutePath(outputPath, outputBasepath);
	//readFile(content, outputFilename, &errno, parentIndex);
	lib_readFile(outputPath, parentIndex, content, &errno);
	if(errno == 0 || errno == -2) // file outputnya sudah ada :v
	{
		print("failed to create hard link: File exists\n");
		return;
	}

	/* DI BAGIAN INI, FILE INPUT MAUPUN OUTPUT SUDAH VALID */
	lib_readSector(dir, 0x101);
	lib_readSector(dir + 512, 0x102);
	
	inputFileIdx = getParent(inputPath, parentIndex);
	
	outputParentIndex = getParent(outputBasepath, parentIndex);
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
		print("failed to create hard link: No empty space\n");
		return;
	}
	dir[errno << 4] = outputParentIndex;
	if (isSoft)
	{
		// softlink
		dir[(errno << 4) + 1] = 0x20 + inputFileIdx;
	}
	else
	{
		// hardlink
		dir[(errno << 4) + 1] = dir[(inputFileIdx << 4) + 1]; // menunjuk ke sektor yang sama
	}
	// copy filename to buffer
	strncpy(dir + (errno << 4) + 2, outputFilename, 14);
	
	lib_writeSector(dir, 0x101);
	lib_writeSector(dir + 512, 0x102);
	return;
}

int main(){
}