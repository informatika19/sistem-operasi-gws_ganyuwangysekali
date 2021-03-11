#include "progs.h"
#include "parse.h"
#include "stds.h"


// path singular relatif terhadap curDirIdx
// path = .   
// path = a   (parent path = curDirIdx)
// path = ..
int isDirectory(const char* path, char curDirIdx)
{
	if(!strcmp(path, ".")) return 1;
	if(!strcmp(path, "..")) return 1;
	
	char *dummy;
	int i;
	for (i = 0; i <= 63; i++)
	{
		if(files[i] == curDirIdx)
		{
			// nama folder == path
			if(!strcmp(strcpy(dummy, files[i + 2], 14), path))
			{
				// sectornya 0xFF dan negatif adalah folder
				return (files[i + 1] == 0xFF || files[i + 1] < 0);
			}
		}
	}
}

void ln(const char *linkedFileName, char *outputFileName)
{
	char* Dir;
	char curDirIdx = sector[511];
	char *content;
	int result;
	
	char* inputPath[255] = parse(linkedPath, SLASH);
	int i = 0;
	while(inputPath[i + 1] != NULL)
	{
		if(chdir(inputPath[i]) == 1) // error
		{
			return;
		}
		i++;
	}
	if(isDirectory(inputPath[i], curDirIdx)) // error
	{
		printString("You cannot link to a folder!");
		return;
	}
	// pasti file
	
	// readFile(content, inputPath[i], &result, sector[511]);
	getcwd(Dir);
	char inputFileSector = *(Dir + 1);
	char inputDirIdx = sector[511];
	
	char* outputPath[255] = parse(outputFilePath, SLASH);
	sector[511] = curDirIdx;
	i = 0;
	while(outputPath[i + 1] != NULL)
	{
		if (chdir(inputPath[i]) == 1) // error
		{
			return;
		}
		i++;
	}
	getcwd(Dir);
	char outputDirPar = *Dir;
	
	/* buat file baru */
	// mungkin writeFile(content, outputPath[i], &result, sector[511]);
	int j = 0;
	while(files[j] != NULL)
	{
		j++;
	}
	files[j] = outputDirPar;
	files[j + 1] = inputFileSector; // isinya sama
	strcpy(&files[j + 2], outputPath[i], 14);
	
	// writeSector files???
	
	// reset
	sector[511] = curDirIdx;
}

// utk soft link (ln -s)
void softln(const char *linkedPath, char *outputFilePath)
{
	char *Dir;
	getcwd(Dir);
	char curDirPar = *Dir;
	char curDirIdx = sector[511];
	
	char* inputPath[255] = parse(linkedPath, SLASH);
	// cari index dengan cd berulang :)
	int i = 0;
	while(inputPath[i + 1] != NULL)
	{
		if (chdir(inputPath[i]) == 1) // error
		{
			return;
		}
		i++;
	}
	if(isDirectory(inputPath[i], sector[511])) chdir(inputPath[i]); // ngelink folder
	else // ngelink file
	{
		sector[511] = curDirIdx;
		ln(linkedPath, outputFilePath);
		return;
	}
	// pasti ngelink folder :v
	
	getcwd(Dir);
//	char inputDirPar = *Dir;
	char inputDirIdx = sector[511];
	
	char* outputPath[255] = parse(outputFilePath, SLASH);
	sector[511] = curDirIdx;
	i = 0;
	while(outputPath[i + 1] != NULL)
	{
		if (chdir(inputPath[i]) == 1) // error
		{
			return;
		}
		i++;
	}
	getcwd(Dir);
	char outputDirPar = *Dir;
	
	/* buat file baru */
	int j = 0;
	while(files[j] != NULL)
	{
		j++;
	}
	files[j] = outputDirPar;
	files[j + 1] = -inputDirIdx; // simpan -index ke sector untuk "menyimpan" linknya
	strcpy(&files[j + 2], outputPath[i], 14);
	
	// writeSector files???
	
	// reset
	sector[511] = curDirIdx;
}

int main(int argc, char *argv[])
{
	if(argc == 3)
	{
		ln(argv[1], argv[2]);
	}
	else if(argc == 4)
	{
		softln(argv[2], argv[3]);
	}
}
