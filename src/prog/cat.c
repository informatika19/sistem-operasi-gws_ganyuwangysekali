#include "progs.h"
#include "stds.h"
#include "file.h"

void cat(char *path, char parentIndex)
{
	char* content;
	int errno;
	readFile(content, path, &errno, parentIndex);
	
	if(errno == -1) // file not found
	{
		printString("No such file or directory");
	}
	else if(errno == -2)
	{
		printString("Is a directory");
	}
	else
	{
		printString(content);
	}
	return;
}

int main(int argc, char *argv[])
{
	cat(argv[1]);
}
