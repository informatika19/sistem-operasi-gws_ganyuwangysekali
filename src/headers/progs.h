#ifndef _PROGS_H
#define _PROGS_H

char map[512];
readSector(map, 0x100);

char files[1024];
readSector(files, 0x101);
readSector(files + 512, 0x102);

char sector[512];
readSector(sector, 0x103);

sector[511] = 0xFF;
map[511] = 1;

void cat(const char *fileName); // cat
int chdir(const char* path); // cd
void ln();
char *getcwd(char *buf);
#endif
