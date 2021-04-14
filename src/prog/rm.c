#include "stds.h"
#include "string.h"
#include "buffer.h"
#include "file.h"

// menangani kasus rekursif
void removebyIndex(char index, char** files, char** sectors, char** maps)
{
    int i;
    char P = (*files)[(index << 4)];
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

    // sebuah folder
    for(i = 0; i < 0x40; i++)
    {
        if((*files)[i << 4] == index)
        {
            removeByIndex(i, files, sectors, maps);
        }
    }
}

// idealnya argc >= 3
void remove(int argc, char* args[], char parentIndex)
{
    char files[1024], map[512], sectors[512];

    int i, j, flagIdx;
    unsigned char isRecursive = 0;
    char* errorMessage;
    char idx;
    // rm
    if(argc == 1)
    {
        interrupt(0x21, 0, "missing operand", 0, 0);
        return;
    }

    // args[0] pasti "rm"
    for(i = 1; i < argc; i++)
    {
        if(!strncmp(args[i], "-r", 2))
        {
            isRecursive = 1;
            flagIdx = i;
            break;
        }
    }

    if(argc == 2 && isRecursive)
    {
        interrupt(0x21, 0, "missing operand", 0, 0); // pesan error pas dicoba di ubuntu
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
    // argc != 2 atau !isRecursive
    if(isRecursive) // argc > 2
    {
        for(i = 1; i < argc; i++)
        {
            if(i == flagIdx) continue;

            getPathIndex(args[i], parentIndex);
            removebyIndex(idx, &files, &sectors, &map);
        }
    }
    else // gak rekursif
    {
        for(i = 1; i < argc; i++)
        {
            getPathIndex(args[i], parentIndex);

            // is a folder
            if(files[(idx << 4) + 1] == 0xFF)
            {
                strncpy(errorMessage, args[i], strlen(args[i]));
                strncat(errorMessage, "Is a directory\n", 15);
                interrupt(0x21, 0, errorMessage, 0, 0);
                continue;
            }

            // kasus symlink
            if(files[(idx << 4) + 1] >= 0x20)
            {
                for(j = 0; j < 16; j++)
                {
                    files[(idx << 4) + j] = 0x00;
                }
                continue;
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