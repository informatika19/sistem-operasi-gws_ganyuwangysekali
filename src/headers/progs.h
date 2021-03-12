#ifndef _PROGS_H
#define _PROGS_H

// pas shell dijalankan (which is pas ngeinclude filenya), diinisiasi root
char cwdIdx = 0xFF;

void cat(char *path, char parentIndex); // cat
char chdir(char* path, int* result, char parentIndex); // cd

char *getcwd(char *buf);
void ls(char *content, char parentIndex); // ls

void ln(char *path, char *outputPath, char parentIndex);
void softln(char *path, char *outputPath, char parentIndex);

#endif
