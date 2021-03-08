#include "../headers/progs.h"

void ls(char *content)
{
	char *cwd;
	getcwd(cwd);
	
	// bisa diganti langsung dengan sector[511] == 0xFF
	if(cwd == NULL) // root
	{
		for(i = 0; i <= 63; i++)
		{
			if (files[16 * i] == 0xFF)
			{
				strcat(content, &files[(16 * i) + 2], 14);
				strcat(content, "\n", 1);
			}
		}
	}
	else // currentDirectory ada
	{
		for(i = 0; i <= 63; i++)
		{
			// parent dari isifile = direktori sekarang
			// *cwd juga bisa digantiin lgsg dengan sector[511]
			if (files[16 * i] == *cwd)
			{
				strcat(content, &files[(16 * i) + 2], 14);
				strcat(content, "\n", 1);
			}
		}
	}
	strcat(content, "\0", 1);
}

int main(int argc, char *argv[])
{
	int i;
	char *content;
	ls(content);
	printString(content);
}
