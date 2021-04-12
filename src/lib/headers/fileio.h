#ifndef _FILEFOLDERIO_H
#define _FILEFOLDERIO_H

char getParent(char* name, char parent);
void createFolder(char* name, char parent, int* err);
void createFile(char* name, char parent, int* err);
void writeFile(char* name, char parent, char* buffer, int* err);
void readFile(char* name, char parent, char* buffer, int* err);
void deleteFolder(char* name, char parent, int* err);
void deleteFile(char* name, char parent, int* err);

#endif //_FILEFOLDERIO_H