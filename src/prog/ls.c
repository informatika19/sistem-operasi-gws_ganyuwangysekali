#include "string.h"
#include "file.h"

void ls(int argc, char* args[], char* buffer, int* result, char parentIndex)
{
	char files[1024];
	int i;
	// ls
	if(argc == 1)
	{
		*result = 0;
	}
	else // ls [path]
	{
		// seolah-olah cd [path]
		parentIndex = chdir(2, args, result, parentIndex);
	}

	if(*result == 2) return; // No such file or directory

	readSector(files, 0x101);
	readSector(files + 512, 0x102);

	if(*result == 1) // ls [file] = file
	{
		parentIndex = getPathIndex(args[1], parentIndex);
		strncat(buffer, files + (parentIndex << 4) + 2, 14);
		strncat(buffer, "\n", 1);
		return;
	}

	// *result == 0
	for(i = 0; i <= 0x3F; i++)
	{
		// parent dari isifile = direktori sekarang
		if (files[i << 4] == parentIndex && files[(i << 4) + 2] != 0)
		{
			strncat(buffer, files + (i << 4) + 2, 14);
			strncat(buffer, "\n", 1);
		}
	}
}

int main()
{
	char *content;
	int errno;
	ls(argc, argv, content, &result, cwdIdx);

	if(result == 1 || result == 0)
	{
		printString(content);
	}
	else
	{
		printString("No such file or directory");
	}
}