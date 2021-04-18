#include "stds.h"
#include "string.h"
#include "buffer.h"
#include "fileio.h"

void remove(char* args, int* errno, char parentIndex)
{
    char files[1024], map[512], sectors[512], path[128];
    char commands[128];
    int i, j;
    unsigned char isRecursive = 0, valid = 0;
    char idx, S;

    clear(path, 128);
    while(*args == ' ') args++;
	valid = 0;
	while(*args != 0){
		*errno = 0;
		clear(commands, 128);

        // cari panjang "sebuah" argv (dalam konteks argc argv)
		while(*(args + (*errno)) != ' ' && *(args + (*errno)) != 0) (*errno)++;

		strncpy(commands, args, (*errno));
		if(strncmp(commands, "-r", 2) == 0) isRecursive = 1;
		else if(strlen(path) == 0) strncpy(path, commands, 128);
		else{
			*errno = 0;
			break;
		}
		valid++; // banyaknya command yang sah
		args += (*errno) + 1;
	}

    // tidak support menghapus banyak file
    if((*errno == 0) || (valid < 1) || (valid > 2))
    {
        // salah penggunaan
		print("Usage: rm [-r] <input>\n");
		return;
	}

    lib_readSector(map, 0x100);
    lib_readSector(files, 0x101);
    lib_readSector(files + 512, 0x102);
    lib_readSector(sectors, 0x103);

    idx = getParent(path, parentIndex);
    if(isRecursive) // valid == 2
    {
        removeIndex(idx, errno, &files, &sectors, &map);
    }
    else // gak rekursif
    {
        // is a folder, tidak dihapus
        if(files[(idx << 4) + 1] == 0xFF)
        {
            *errno = -2; // Is a directory
            print("Is a directory\n");
            return;
        }

        // sudah pasti bukan folder
        // removeIndex pasti tidak rekursif
        removeIndex(idx, errno, &files, &sectors, &map);
    }
    lib_writeSector(map, 0x100);
    lib_writeSector(files, 0x101);
    lib_writeSector(files + 512, 0x102);
    lib_writeSector(sectors, 0x103);
}

int main()
{
}