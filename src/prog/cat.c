#include "progs.h"
#include "parse.h"
#include "stds.h"
#include "file.h"

void cat(char *path, char parentIndex)
{
	char* content;
	int errno;
	readFile(content, path, &errno, parentIndex);
	
	if(errno == 0)
	{
		printString(content);
	}
	else if(errno == -1) // file not found
	{
		printString("No such file or directory");
	}
	else if(errno == -2)
	{
		printString("Is a directory");
	}
}

int main(int argc, char *argv[])
{
	cat(argv[1]);
}
