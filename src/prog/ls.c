#include "../headers/progs.h"

// kemungkinan besar alternatif 2
int main(int argc, char *argv[])
{
	int i;
	char *currentDirectory;
	getcwd(currentDirectory);
	char *content;
	
	if(currentDirectory == NULL) // root
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
			if (files[16 * i] == *currentDirectory)
			{
				strcat(content, &files[(16 * i) + 2], 14);
				strcat(content, "\n", 1);
			}
		}
	}
	
	printString(content);
}
