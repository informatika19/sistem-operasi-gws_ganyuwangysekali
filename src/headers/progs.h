#ifndef _PROGS_H
#define _PROGS_H

void cat(char *inputPath, char parentIndex);

// cd [path]
char chdir(char* inputPath, char parentIndex); 

// ls [path]
void ls(char* argv, char parentIndex);

// ln [path1] [path2]
// ln [path1] [flags] [path2]
void ln(char* args, char parentIndex);

// rm [path]
// rm [path] -r atau rm -r [path]
void remove(char* args, int* errno, char parentIndex);

// copy [file] [output]
// copy [-r] [path] [output]
void copy(char* args, char parentIndex);

#endif