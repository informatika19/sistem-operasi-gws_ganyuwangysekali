#include "progs.h"

int chdir(const char* path)
{
	char *cwd;
	getcwd(cwd);
	int curDirIdx = *cwd;
	// kasus spesial :)
	
	if(strcmp(path, ".") == 0) return 0;
	if(strcmp(path, "..") == 0)
	{
		if (cwd == NULL) return 1; // error
		
		// ke parent dari cwd
		sector[511] = files[curDirIdx * 16];
		return 0;
	}
		
	if (cwd == NULL) // root
	{
		// cari yang nama filenya sama dengan path dan parentIndexnya adalah 0xFF
		for(j = 0; j <= 63; j++)
		{
			if(files[16 * j] == 0xFF && strcmp(strcpy(dummy, files[16 * j + 2], 14), path) == 0)
			{
				sector[511] = files[16 * j];
				return 0;
			}
		}
	}
	else
	{
		// cari yang nama filenya sama dengan path dan parentIndexnya adalah cwd[0]
		for(j = 0; j <= 63; j++)
		{
			if(files[16 * j] == *cwd && strcmp(strcpy(dummy, files[16 * j + 2], 14), path) == 0)
			{
				sector[511] = files[16 * j];
				return 0;
			}
		}
	}
	return 1;

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
	char* args[255] = parse(argv[1], SLASH);
	
	int i = 0, j;
	while (args[i] != NULL && i < 255)
	{
		if(chdir(args[i]) == 1) return 1; // error??
		// directorynya udah berubah karena chdir udah dipanggil
		i++;
	}
	return 0;
}
