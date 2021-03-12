#ifndef _PROGS_H
#define _PROGS_H

// pas shell dijalankan (which is pas ngeinclude filenya), diinisiasi root
char cwdIdx = 0xFF;

void cat(char *fileName); // cat
char chdir(char* path, int* result, char parentIndex); // cd

char *getcwd(char *buf);
void ls(char* content);

int isDirectory(const char* path, char curDirIdx);
void ln(const char *linkedFileName, char *outputFileName);
void softln(const char *linkedPath, char *outputFilePath);

#endif
