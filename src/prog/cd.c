#include "progs.h"
#include "parse.h"
#include "stds.h"
#include "file.h"

// parentIndex disimpan di cwdIdx
char chdir(char* path, int* result, char parentIndex)
{
	char dir[1024];
	char currentName[14];
	char traversingName[14];
	int i;
	char tempIdx;
	
	readSector(dir, 0x101);
	readSector(dir+512, 0x102);
	
	if(*path == '/')
	{
		*result = 0;
		tempIdx = chdir(path + 1, result, parentIndex);
	}
	
	if(path != 0)
	{
		// parse the path one by one
		
		// './'
		if(*path == '.' && *(path+1) == '/')
		{
			// cd sukses
			*result = 0;
			tempIdx = chdir(path + 2, result, parentIndex);
		}
		
		// '/'
		if(*path == '/')
		{
			// cd sukses
			*result = 0;
			tempIdx = chdir(path + 1, result, parentIndex); 
		}
		
		// '../'
		if(*path == '.' && *(path+1) == '.' && *(path+2) == '/')
		{ 
			*result = 0;
			if(parentIndex == 0xFF) parentIndex = 0xFF;
			else parentIndex = dir[parentIndex << 4];
			
			tempIdx = chdir(path + 3, result, parentIndex);
		}
		
		// ambil nama file/folder nya
		i = 0;
		while(*path != '/' && *path != 0 && i < 14)
		{
			traversingName[i++] = *(path++);
		}
		if(i < 14) traversingName[i] = 0;
		if(*path != '/' && *path != 0)
		{
			// path invalid, more than 14 bytes long
			*result = 3;
			return parentIndex;
		}
		
		char trueParentIndex = parentIndex;
		while(dir[trueParentIndex << 4 + 1] > 0x1F)
		{
			trueParentIndex = dir[trueParentIndex << 4 + 1] - 0x20;
		}
		
		// nama filenya valid, cari indeks yang bersangkutan
		i = 0;
		do
		{ // compare the name with all names in dir
			strncpy(currentName, dir + (i << 4) + 2, 14);
			i++;
		}
		while(strcmp(currentName, traversingName) != 0 && dir[i << 4] != trueParentIndex && i < 0x40);
		
		if(i == 0x40)
		{
			*result = 2;
			return parentIndex;
		} // path not in dir
		
		// not a folder
		if(dir[i << 4 + 1] != 0xFF)
		{	
			*result = 1;
			return parentIndex;
		}
		
		// cdnya sukses
		*result = 0;
		tempIdx = chdir(path, result, i);
	}
	
	if(*result == 0)
	{
		return tempIdx;
	}
	return parentIndex;
}

int main(int argc, char *argv[])
{
	int errno = 0;
	cwdIdx = chdir(argv[1], &errno, cwdIdx);
	
	if(errno == 1)
	{
		printString("Not a directory");
	}
	else if(errno == 2)
	{
		printString("No such file or directory");
	}
	else if(errno == 3)
	{
		printString("Not a valid file or directory name");
	}
	
	return errno;
}
