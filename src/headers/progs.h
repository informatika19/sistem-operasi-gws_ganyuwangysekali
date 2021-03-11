#ifndef _PROGS_H
#define _PROGS_H

/* inisialisasi SHELL */
/*
	readSector(map, 0x100);
	readSector(files, 0x101);
	readSector(files + 512, 0x102);
	readSector(sector, 0x103);
	sector[511] = 0xFF; //cwd
	map[511] = 1;
*/

void cat(const char *fileName); // cat
int chdir(const char* path); // cd

char *getcwd(char *buf);
void ls(char* content);

int isDirectory(const char* path, char curDirIdx);
void ln(const char *linkedFileName, char *outputFileName);
void softln(const char *linkedPath, char *outputFilePath);

#endif
