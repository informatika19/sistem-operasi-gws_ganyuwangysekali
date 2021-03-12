#include "file.h"
#include "string.h"
#include "buffer.h"

void readSector(char *buffer, int sector)
{
	interrupt(0x13, 0x0201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector)
{
	interrupt(0x13, 0x0301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex)
{
	char map[512], dir[1024], sect[512], filename[14], basepath[512], realParentIndex;
	int i = 0, j, k;
	unsigned char valid = 0;

  	getFilename(path, filename);
  	getBasePath(path, basepath, parentIndex);
  	realParentIndex = getPathIndex(basepath, parentIndex);
  
	readSector(dir, 0x101);
	readSector(dir+512, 0x102);
	while(i < 0x40 && (dir[i << 4 + 1] == 0xFF || dir[i << 4 + 1] < 0x40))
	{
		if(dir[i << 4] == realParentIndex && strncmp(dir + i << 4 + 2, filename, 14) != 0)
		{
			// file sudah ada :D
			*sectors = -1;
			return;
		}
		if(i == parentIndex) valid = 1;
		i++;
	}
	if(i > 0x3F)
	{
		// tidak ada dir kosong
		*sectors = -2;
		return;
	}
	if(!valid)
	{
		// folder tidak valid
    		*sectors = -4;
    		return;
	}
	
	// baca map
	readSector(map, 0x100);
	j = 0;
	k = 0;
	while(j < *sectors && k < 512)
	{
		if(map[k++] != 0xFF) j++;
	}
	
	if(j < *sectors)
	{
		// map sudah penuh
		*sectors = -3;
		return;
	}
	
	// baca sectors
	readSector(sect, 0x103);
	while(k < 0x20 && sect[k<<4] != 0) k++;
	
	if(k == 0x20)
	{
		// sectors ga muat
		*sectors = -3;
		return;
	}
	
	// clear buffer files
	clear(dir+(4<<i), 16);
	
	// put parent index
	dir[(4<<i)] = realParentIndex;
	
	// put sector index
	dir[(4<<i)+1] = k;
	
	// copy filename to buffer
	strncpy(dir+(4<<i)+2, filename, 14);
	
	// write buffer to sector
	j = 0;
	i = 0;
	while(*sectors > 0)
	{
		if(map[j] != 0xFF)
		{
			i++;
			writeSector(buffer, j);
			sect[(k<<4)+i] = j;
			map[j] = 0xFF;
		}
		j++;
	}
}

void readFile(char *buffer, char *path, int *result, char parentIndex)
{
	char sector[512], dir[1024], sect[512];
	int i;
	readSector(dir, 0x101);
	readSector(dir+512, 0x102);
	// TODO: implement readFile
}

char getPathIndex(char* path, char parentIndex)
{
	char dir[1024];
	char currentName[14];
	char traversingName[14];
	int i;
	
	readSector(dir, 0x101);
	readSector(dir+512, 0x102);
	
	// traverse the path
	if(*path == '/')
	{
		// skip the first char if root
		parentIndex = 0xFF;
		path++;
	}
	
	while(path != 0)
	{ 
		// let's parse the path until the very end
		if(*path == '.' && *(path+1) == '/') path += 2; // './'
		if(*path == '/') path++; // '/'
		if(*path == '.' && *(path+1) == '.' && *(path+2) == '/')
		{ // '../'
			if(parentIndex == 0xFF) parentIndex = 0xFF;
			else parentIndex = dir[parentIndex<<4];
		}
		i = 0;
		while(*path != '/' && *path != 0 && i < 14)
		{
			traversingName[i++] = *(path++);
		}
		if(i < 14) traversingName[i] = 0;
		if(*path != '/' && *path != 0)
		{
			// path invalid, more than 14 bytes long
			return 0xFE;
		}
		i = 0;
		do
		{ // compare the name with all names in dir
			strncpy(currentName, dir + (i << 4) + 2, 14);
			i++;
		}
		while(strcmp(currentName, traversingName) != 0 && dir[i << 4] != parentIndex && i < 0x40);
		
		if(i == 0x40) return 0xFE; // path not in dir
		parentIndex = i;
		if(dir[parentIndex << 4 + 1] != 0xFF) break;
	}
	return parentIndex;
}

void getBasePath(char* path, char* basepath, char parentIndex)
{
	char currentName[15];
	int i;
	while(*path != 0)
	{
		i = 0;
		while(*path != '/' && *path != 0)
		{
			currentName[i++] = *(path++);
		}
		currentName[i++] = '/';
		if(*path == 0) break;
		if(i < 15) currentName[i] = 0;
		strncpy(basepath, currentName, 14);
		basepath += strlen(currentName);
		path++;
	}
}

void getFilename(char* path, char* filename)
{
	while(*path != 0) path++;
	while(*path != '/') path--;
	strncpy(filename, path+1, 14);
}
