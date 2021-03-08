#include "progs.h"

void cat(const char *fileName)
{
	char* content;
	int result;
	char* dummy;
	/*
	
	// apakah readFile memitigasi fileName yang ada directory?
	readFile(content, fileName, &result, sector[511]);
	
	if (result != -1)
	{
		printString(content);
		return;
	}	
	*/
	
	/* ALTERNATIF LAIN */
	/* CD berulang, lalu readFile yang bersangkutan */
	char *args[255] = parse(fileName, SLASH);
	int i = 0, j;
	while(args[i + 1] != NULL)
	{
		result = chdir(args[i]);
		if (result == 1) // error
		{
			return;
		}
		i++;
	}
	readFile(content, args[i], &result, sector[511]);
	
	if (result == -1)
	{
		// error
		return;
	}	
	printString(content)
}

int main(int argc, char *argv[])
{
	char curDirIdx = sector[511];
	cat(argv[1]);
	sector[511] = curDirIdx; // reset cwd
}
