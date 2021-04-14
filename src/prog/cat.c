#include "fileio.h"
#include "basicio.h"

void cat(char *inputPath, char parentIndex)
{
	char content[8192];
	int errno;
	while(*(inputPath) == ' ') inputPath++;
	if(*inputPath == 0){
		print("Usage: cat <filename>\n");
		return;
	}
	lib_readFile(inputPath, parentIndex, content, &errno);
	if(errno == -1) // file not found
	{
		print("No such file or directory\n");
	}
	else if(errno == -2)
	{
		print("Is a directory\n");
	}
	else
	{
		print(content);
		print("\n");
	}
	return;
}

int main()
{
}