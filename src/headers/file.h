#ifndef _FILE_H
#define _FILE_H
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
char getPathIndex(char* path, char parentIndex);
void getBasePath(char* path, char* basepath, char parentIndex);
void getFilename(char* path, char* filename);
#endif