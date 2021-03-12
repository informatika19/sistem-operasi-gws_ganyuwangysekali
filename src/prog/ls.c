#include "progs.h"
#include "stds.h"

void ls(char *content, char parentIndex)
{
	for(i = 0; i <= 0x3F; i++)
	{
		// parent dari isifile = direktori sekarang
		if (files[16 * i] == parentIndex)
		{
			strncat(content, files + (i << 4) + 2, 14);
			strncat(content, "\n", 1);
		}
	}
	strncat(content, 0, 1);
}

int main(int argc, char *argv[])
{
	char *content;
	ls(content, cwdIdx);
	printString(content);
}
