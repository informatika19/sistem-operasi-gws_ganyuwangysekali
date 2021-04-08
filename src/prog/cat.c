#include "progs.h"
#include "stds.h"
#include "file.h"

void cat(int argc, char* args[], char parentIndex)
{
	char content[8192];
	int errno;
	if(argc == 0){
		// interaksinya di ubuntu adalah readString lalu printString
		// sampai diinterupsi, tapi tidak akan diimplementasi
		return;
	}

	// cat [file]
	readFile(content, args[1], &errno, parentIndex);

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
/*
int main(int argc, char *argv[])
{
	cat(argc, argv, cwd dari shell);
}
*/