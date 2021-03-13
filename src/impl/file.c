#include "file.h"
#include "string.h"
#include "buffer.h"
#include "kernel.h"

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

	valid = realParentIndex == 0xFF;
	while(i < 0x40 && (dir[(i<<4) + 1] == 0xFF || dir[(i << 4) + 1] < 0x20)){
		if(dir[i << 4] == realParentIndex && strncmp(dir+(i<<4)+2, filename, 14) != 0){
			// file sudah ada :D
			*sectors = -1;
			return;
		}
		if(i == realParentIndex) valid = 1;
		i++;
	}
	i = 0;
	while(i < 0x40 && (dir[(i<<4)+2] != 0)) i++;
	if(i > 0x3F){
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
	k = 0;
  while(k < 0x20 && sect[k<<4] != 0) k++;
  if(k == 0x20){
    // sectors ga muat
    *sectors = -3;
    return;
  }

  // clear buffer files
  clear(dir+(i<<4), 16);
  // put parent index
	dir[(i<<4)] = realParentIndex;
  // put sector index
	dir[(i<<4)+1] = k;
  // copy filename to buffer
  strncpy(dir+(i<<4)+2, filename, 14);
  // write buffer to sector
	writeSector(dir, 0x101);
	writeSector(dir+0x100, 0x102);
	j = 0;
	i = 0;
	while(*sectors > 0)
	{
		if(map[j] != 0xFF)
		{
			writeSector(buffer+(i<<8), j);
			sect[(k<<4)+i] = j;
			map[j] = 0xFF;
			i++;
			*sectors--;
		}
		j++;
	}
	writeSector(map, 0x100);
	writeSector(sect, 0x103);
}

void readFile(char *buffer, char *path, int *result, char parentIndex)
{
	char dir[1024], sect[512], basepath[512], filename[14], realParentIndex, S;
	int i = 0, j, found = 0;

	getBasePath(path, basepath, parentIndex);
	realParentIndex = getPathIndex(basepath, parentIndex);
	if (realParentIndex == 0xFE) {
		// file ga ketemu D:
		*result = -1;
		return;
	}

	readSector(dir, 0x101);
	readSector(dir+512, 0x102);
	readSector(sect, 0x103);

	S = dir[realParentIndex * 16 + 1];
	if (S == 0xFF || S >= 0x40) {
		// yang kebaca itu folder
		*result = -2;
		return;
	}

	while (found == 0 && i < 16) {
		j = i + S * 16;
		if (sect[j] == 0x00) {
			found = 1;
		}
		readSector(buffer + (512 * j), sect[j]);
		i++;
	}
}

char getPathIndex(char* path, char parentIndex)
{
	char dir[1024];
	char currentName[14];
	char traversingName[14];
	int i;
	
	readSector(dir, 0x101);
	readSector(dir+512, 0x102);

	if(*path == 0) return 0xFF;
	
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
			if(parentIndex != 0xFF) parentIndex = dir[parentIndex<<4];
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
	*basepath = 0;
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
	char* mpath = path;
	while(*mpath != 0) mpath++;
	while(*mpath != '/' && mpath != path) mpath--;
	if(*mpath == '/') mpath++;
	strncpy(filename, mpath, 14);
}