#include "basicio.h"
#include "fileio.h"

void cat(char *inputPath, char parentIndex);

int main()
{
	char buffer[512 << 4];
	char parent, arg[512];
	int err;
	lib_readFile(buffer, "tempc", &err, 0xFF);
	removeFEntry("tempc", 0xFF, &err);
	parse(buffer, &parent, arg);

	cat(arg, parent);

	clear(buffer);
	buffer[0] = parent;
	err = 16;
	lib_writeFile(buffer, "tempc", &err, 0xFF);
	
	exec("/bin/shell", 0xFF, &err);
}

void cat(char *inputPath, char parentIndex)
{
	char content[8192];
	int errno;
	while(*(inputPath) == ' ') inputPath++;
	if(*inputPath == 0){
		print("Usage: cat <filename>\n");
		return;
	}
	lib_readFile(content, inputPath, &errno, parentIndex);
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