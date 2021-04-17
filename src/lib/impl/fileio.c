#include "string.h"

void lib_readFile(char* buffer, char* name, int* err, char parent)
{
  interrupt(0x21, (parent << 8)|0x04, buffer, name, err);
}

void lib_writeFile(char* buffer, char* name, int* err, char parent)
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

void exec(char* name, char parent, int* err)
{
  interrupt(0x21, (parent << 8) | 0x06, name, 0x3000, err);
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
    if(files[(i << 4) + 2] == 0) return 0xFE;
 		parent = i;
 	}
  return parent;
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

// errorcode mengikuti WRITEFILE
// tambah errorcode -5, BUAT FOLDER DI DALAM FILE
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

    // mau buat folder di dalam file
    if(files[(parent << 4) + 1] != 0xFF)
    {
      *err = -5;
      return;
    }

    files[i << 4] = parent;
    files[(i << 4) + 1] = 0xFF;
    strncpy(files + (i << 4) + 2, fileName, 14);

    lib_writeSector(files, 0x101);
    lib_writeSector(files + 512, 0x102);
  }
}

void createFile(char* name, char parent, int* err)
{
  lib_writeFile("", name, err, parent);
}

void removeIndex(char index, int* errno, char** files, char** sectors, char** maps)
{
    int i;
    char S = (*files)[(index << 4) + 1];

    if(S >= 0x20) // kasus softlink
    {
        for(i = 0; i < 16; i++)
        {
            (*files)[(index << 4) + i] = 0x00;
        }
        return;
    }

    // bukan sebuah folder
    if(S != 0xFF)
    {
        for(i = 0; i < 16; i++)
        {
            (*files)[(index << 4) + i] = 0x00;
        }
        for(i = 0; i < 16; i++)
        {
            (*sectors)[(S << 4) + i] = 0x00;
            (*maps)[(S << 4) + i] = 0x00;
        }
        return;
    }

    // sebuah folder (hapus semua anaknya secara rekursif)
    for(i = 0; i < 0x40; i++)
    {
        if((*files)[i << 4] == index)
        {
            removeIndex(i, errno, files, sectors, maps);
        }
    }
    *errno = 1;
}

int countEmptyFile()
{
  char files[1024], i;
  unsigned char result = 0;

  lib_readSector(files, 0x101);
  lib_readSector(files + 512, 0x102);

  for(i = 0; i < 0x40; i++)
  {
    if(files[(i << 4) + 2] == 0) result++;
  }
  return result;
}

int countEmptySector()
{
  char sectors[512], i;
  unsigned char result = 0;

  lib_readSector(sectors, 0x103);
  for(i = 0; i < 0x20; i++)
  {
    if(sectors[i << 4] == 0) result++;
  }
  return result;
}
