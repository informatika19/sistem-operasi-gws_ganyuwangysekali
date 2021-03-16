#ifndef _PROGS_H
#define _PROGS_H

void cat(char *inputPath, char parentIndex);
char chdir(char* inputPath, int* result, char parentIndex); // cd

void ls(char* argv, char parentIndex); // ls

void ln(char* args, char parentIndex);
void softln(char *inputPath, char *outputPath, char parentIndex);

#endif
