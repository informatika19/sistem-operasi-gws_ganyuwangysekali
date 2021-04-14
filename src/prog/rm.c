#include "stds.h"
#include "string.h"
#include "buffer.h"

void removebyIndex(char index, char** files, char** sectors, char** maps)
{
    int i;
    char S = (*files)[(index << 4) + 1];
    if(!(index >= 0x00 && index < 0x40))
    {
        interrupt(0x21, 0, "No such file or directory", 0, 0);
        return;
    }

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
            removeByIndex(i, files, sectors, maps);
        }
    }
}

void remove(char* args, char parentIndex)
{
    char files[1024], map[512], sectors[512], path[128];
    char commands[128];
    int i, j, len;
    unsigned char isRecursive = 0, valid = 0;
    char idx;

    clear(path, 128);
    while(*args == ' ') args++;
	valid = 0;
	while(*args != 0){
		len = 0;
		clear(commands, 128);

        // cari panjang "sebuah" argv (dalam konteks argc argv)
		while(*(args + len) != ' ' && *(args + len) != 0) len++;

		strncpy(commands, args, len);
		if(strncmp(commands, "-r", 2) == 0) isRecursive = 1;
		else if(strlen(path) == 0) strncpy(path, commands, 128);
		else{
			len = -1;
			break;
		}
		valid++; // banyaknya command yang sah
		args += len + 1;
	}

    // tidak support menghapus banyak file
    if((len == -1) || (valid < 1) || (valid > 2))
    {
		interrupt(0x21, 0, "Usage: rm [-r] <input>\n", 0, 0);
		return;
	}

//    readSector(map, 0x100);
//    readSector(files, 0x101);
//    readSector(files + 512, 0x102);
//    readSector(sectors, 0x103);
    interrupt(0x21, 0x0002, map, 0x100, 0);
    interrupt(0x21, 0x0002, files, 0x101, 0);
    interrupt(0x21, 0x0002, files + 512, 0x102, 0);
    interrupt(0x21, 0x0002, sectors, 0x103, 0);

    idx = getPathIndex(path, parentIndex);
    if(isRecursive) // valid == 2
    {
        removebyIndex(idx, &files, &sectors, &map);
    }
    else // gak rekursif
    {
        // is a folder, tidak dihapus
        if(files[(idx << 4) + 1] == 0xFF)
        {
            interrupt(0x21, 0, "Is a directory\n", 0, 0);
            return;
        }

        // kasus symlink (ada dihapus, jadi perlu writeSector)
        if(files[(idx << 4) + 1] >= 0x20)
        {
            for(j = 0; j < 16; j++)
            {
                files[(idx << 4) + j] = 0x00;
            }
            interrupt(0x21, 0x0003, files, 0x101, 0);
            interrupt(0x21, 0x0003, files + 512, 0x102, 0);
            return;
        }

        // pasti sebuah file
        for(j = 0; j < 16; j++)
        {
            sectors[(files[(idx << 4) + 1] << 4) + 1] = 0x00;
            map[(files[(idx << 4) + 1] << 4) + 1] = 0x00;
        }
        for(j = 0; j < 16; j++)
        {
            files[(idx << 4) + j] = 0x00;
        }
    }
//    writeSector(map, 0x100);
//    writeSector(files, 0x101);
//    writeSector(files + 512, 0x102);
//    writeSector(sectors, 0x103);
    interrupt(0x21, 0x0003, map, 0x100, 0);
    interrupt(0x21, 0x0003, files, 0x101, 0);
    interrupt(0x21, 0x0003, files + 512, 0x102, 0);
    interrupt(0x21, 0x0003, sectors, 0x103, 0);
}

int main()
{
}