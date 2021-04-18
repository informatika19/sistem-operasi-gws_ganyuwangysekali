#ifndef _FILEFOLDERIO_H
#define _FILEFOLDERIO_H

char getParent(char* name, char parent);
void lib_getFullPath(char* name, char parent, char* out);
void lib_getAbsolutePath(char* name, char* out);
void lib_getFileName(char* name, char* out);
void createFolder(char* name, char parent, int* err);
void createFile(char* name, char parent, int* err);
void lib_writeFile(char* buffer, char* name, int* err, char parent);
void lib_readFile(char* buffer, char* name, int* err, char parent);
void lib_readSector(char *buffer, int sector);
void lib_writeSector(char *buffer, int sector);
void removeIndex(char index, int* errno, char** files, char** sectors, char** maps);
void exec(char* name, char parent, int* err);

#endif