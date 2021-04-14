#include "basicio.h"

#include "fileio.h"

void cat(char *inputPath, char parentIndex)
{
	char content[8192];
	int errno;
	while(*(inputPath) == ' ') inputPath++;
	if(*inputPath == 0){
		printString("Usage: cat <filename>\n");
		return;
	}
	readFile(content, inputPath, &errno, parentIndex);

	if(errno == -1) // file not found
	{
		printString("No such file or directory\n");
	}
	else if(errno == -2)
	{
		printString("Is a directory\n");
	}
	else
	{
		printString(content);
		printString("\n");
	}
	return;
}

int main()
{
	int argc;
	char* argv;
}