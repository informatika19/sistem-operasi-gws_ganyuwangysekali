#ifndef _PROGS_H
#define _PROGS_H

void cat(int argc, char* args[], char parentIndex);

// cd [path]
char chdir(int argc, char* args[], int* result, char parentIndex);

// ls [path]
void ls(int argc, char* args[], char* buffer, int* result, char parentIndex);

// ln [path1] [path2]
// ln [path1] [flags] [path2]
void ln(char* args, char parentIndex);

// rm [path]
void remove(int argc, char* args[], char parentIndex);
// rm [path] -r atau rm -r [path]
void removebyIndex(char index, char** files, char** sectors, char** maps);

// copy [file] [output]
// copy [-r] [path] [output]
void copy(int argc, char* args[], char parentIndex);

#endif