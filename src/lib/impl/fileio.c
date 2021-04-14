#include "string.h"

void lib_readFile(char* name, char parent, char* buffer, int* err)
{
  interrupt(0x21, (parent << 8)|0x04, buffer, name, err);
}

void lib_writeFile(char* name, char parent, char* buffer, int* err)
{
  interrupt(0x21, (parent << 8)|0x05, buffer, name, err);
}

void lib_readSector(char *buffer, int sector)
{
  interrupt(0x21, 0x0002, buffer, sector, 0);
}

void lib_writeSector(char *buffer, int sector)
{
  interrupt(0x21, 0x0003, buffer, sector, 0);
}

char getParent(char* name, char parent)
{
  char currentFileName[14];
  char nextFileName[14];
  int i;
  char files[1024];

  lib_readSector(files, 0x101);
  lib_readSector(files + 512, 0x102);

  if(*name == 0) return parent;

	// traverse the path

  // root
 	if(*name == '/')
  {
 		parent = 0xFF;
 		name++;
 	}

 	while(*name != 0)
 	{
    // prev
    if(*name == '.' && *(name + 1) == '.')
    {
      if(parent != 0xFF)
      {
        parent = files[parent << 4];
      }
      name += 2;
      continue;
    }

    // ignore
 		if(*name == '/')
    { // '/'
 			name++;
 			continue;
 		}

    // "./"
    if(*name == '.' && *(name + 1) == '/')
    {
      name += 2;
      continue;
    }

    // "."
    if(*name == '.' && *(name + 1) == 0)
    {
      name++;
      continue;
    }

	  if(*name == 0) break;

 		i = 0;
    // baca "nama file"
  	while(*name != '/' && *name != 0 && i < 14)
    {
 			nextFileName[i] = *name;
      i++;
 			name++;
 		}
    
  	if(i < 14) nextFileName[i] = 0;

    // nama filenya masih belum habis terbaca :(
  	if(*name != '/' && *name != 0)
    {
 			return 0xFE;
 		}

 		i = 0;
 		while(i < 0x40)
    {
      // cari di sektor files
 			if(files[i << 4] == parent && strncmp(files + (i << 4) + 2, nextFileName, 14) == 0) break;
 			i++;
 		}
 
    // gak ditemukan
 		if(i == 0x40) return 0xFE;
    
    // kasus softlink
 		if(files[(i << 4) + 1] >= 0x20 && files[(i << 4) + 1] != 0xFF) i = (files[(i << 4) + 1] - 0x20);
 		parent = i;    
 	}
  return parent;
}

// errorcode mengikuti WRITEFILE
void createFolder(char* name, char parent, int* err)
{
  char files[1024];
  char* absolutePath, fileName;
  int i;

  lib_getAbsolutePath(name, absolutePath);
  lib_getFileName(name, fileName);
  parent = getParent(absolutePath, parent);

  if(parent == 0xFE) // foldernya gak ketemu
  {
    *err = -4;
  }
  else //parent nya ketemu
  {
    lib_readSector(files, 0x101);
    lib_readSector(files + 512, 0x102);
    
    // cari filenya
    i = 0;
    while(i < 0x40)
    {
      if(!strncmp(fileName, files + (i << 4) + 2, 14) && files[i << 4] == parent)
      {
        *err = -1; // filenya ada
        return;
      }
      else
      {
        i++;
      }
    }

    // filenya pasti gak ada

    // cari indeks kosong di dalam sektor files
    i = 0;
    while(i < 0x40)
    {
      if(files[(i << 4) + 2] == 0) break;

      i++;
    }
    
    // penuh
    if(i == 0x40)
    {
      *err = -2;
      return;
    }

    files[i << 4] = parent;
    files[(i << 4) + 1] = 0xFF;
    strncpy(files + (i << 4) + 2, fileName, 14);

    lib_writeSector(files, 0x101);
    lib_writeSector(files + 512, 0x102);
  }
}

void lib_getAbsolutePath(char* name, char* out)
{
  char* temp = name;
  // cari posisi "/" terakhir
  while(*temp != 0) temp++;
  while(*temp != '/' && temp > name) temp--;

  while(name < temp)
  {
    *out = *name;
    name++;
    out++;
  }
  *out = 0;
}

void lib_getFileName(char* name, char* out)
{
  char* temp = name;
  // cari posisi "/" terakhir
  while(*temp != 0) temp++;
  while(*temp != '/' && temp > name) temp--;

  // temp == name atau *temp == "/"
  if(*temp == '/') temp++;
  while (*temp != 0)
  {
    *out = temp;
    name++;
    out++;
  }
  *out = 0;
}

void createFile(char* name, char parent, int* err)
{
  lib_writeFile(name, parent, "", err);
}

void deleteFolder(char* name, char parent, int* err)
{

}

void deleteFile(char* name, char parent, int* err)
{

}