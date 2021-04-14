#ifndef _FILEFOLDERIO_H
#define _FILEFOLDERIO_H

char getParent(char* name, char parent);
void createFolder(char* name, char parent, int* err);
void createFile(char* name, char parent, int* err);
void lib_writeFile(char* name, char parent, char* buffer, int* err);
void lib_readFile(char* name, char parent, char* buffer, int* err);
void lib_readSector(char *buffer, int sector);
void lib_writeSector(char *buffer, int sector);
void deleteFolder(char* name, char parent, int* err);
void deleteFile(char* name, char parent, int* err);

#endif //_FILEFOLDERIO_H