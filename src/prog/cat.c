#include "progs.h"
#include "stds.h"
#include "file.h"

void cat(char *path, char parentIndex)
{
	char content[8192];
	int errno;
	while(*(path) == ' ') path++;
	if(*path == 0){
		printString("Usage: cat <filename>\n");
		return;
	}
	readFile(content, path, &errno, parentIndex);

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

// int main(int argc, char *argv[])
// {
// 	// cat(argv[1]);
// }
