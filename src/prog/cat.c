#include "progs.h"

void cat(const char *fileName)
{
	/*
	char *buf;
	int sector = fileName--;
	
	int result;
	readFile(buf, fileName, &result, fileName - 2);
	
	if (result == -1)
	{
		// error code?
		return;
	}
	
	printString(buf);
	return;
	*/
}

int main(int argc, char *argv[])
{
	char *args[255] = parse(argv[1], SLASH);
	char curDirIdx = sector[511];
	
	// cari file yang dimaksud pake cd berulang :v
	// lalu baca isinya pakai readFile
	// lalu printString kalau gak error
	
	// reset cwd
	sector[511] = curDirIdx;
}
