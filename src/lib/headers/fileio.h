#ifndef _FILEFOLDERIO_H
#define _FILEFOLDERIO_H

char getParent(char* name, char parent);
void createFolder(char* name, char parent, int* err);
void createFile(char* name, char parent, int* err);
void lib_writeFile(char* name, char parent, char* buffer, int* err);
void lib_readFile(char* name, char parent, char* buffer, int* err);
void lib_readSector(char *buffer, int sector);
void lib_writeSector(char *buffer, int sector);
void removeIndex(char index, int* errno, char** files, char** sectors, char** maps);

#endif //_FILEFOLDERIO_H