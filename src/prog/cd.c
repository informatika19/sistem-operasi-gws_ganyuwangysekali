#include "progs.h"

int chdir(const char* path)
{
	char *dummy;
	/* ALTERNATIF 1 */
	/*
	int errno = interrupt(0x21, 0x3b02, dummy, 0x101, path);
	
	// reference for error code :
	// bbc.nvg.org/doc/Master%20512%20Technical%20Guide/m512techb_int21.htm
	if (errno == 3)
	{
		errno = interrupt(0x21, 0x3b02, dummy, 0x102, path);
	}	
	return errno;
	*/
}

int main(int argc, char *argv[])
{
	/* ALTERNATIF 2 */
	char* args[255] = parse(argv[1], SLASH);
	char* cwd;
	
	int i = 0, j;
	char *dummy;
	while (args[i] != NULL)
	{
		getcwd(cwd);
		int curDirIdx = *cwd;
		// kasus spesial :)
		if(strcmp(args[i], "..") == 0)
		{
			if (cwd == NULL); // error
			
			// ke parent dari cwd
			sector[511] = files[curDirIdx * 16];
			i++;
			continue;
		}
		
		if (cwd == NULL) // root
		{
			// cari yang nama filenya sama dengan args[i] dan parentIndexnya adalah 0xFF
			for(j = 0; j <= 63; j++)
			{
				if(files[16 * j] == 0xFF && strcmp(strcpy(dummy, files[16 * j + 2], 14), args[i]) == 0)
				{
					sector[511] = files[16 * j];
					break;
				}
			}
		}
		else
		{
			// cari yang nama filenya sama dengan args[i] dan parentIndexnya adalah cwd[0]
			for(j = 0; j <= 63; j++)
			{
				if(files[16 * j] == *cwd && strcmp(strcpy(dummy, files[16 * j + 2], 14), args[i]) == 0)
				{
					sector[511] = files[16 * j];
					break;
				}
			}
		}
		i++;
	}
}
