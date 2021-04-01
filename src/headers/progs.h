#ifndef _PROGS_H
#define _PROGS_H

void cat(char *inputPath, char parentIndex);
char chdir(char* inputPath, int* result, char parentIndex); // cd

void ls(char* argv, char parentIndex); // ls

void ln(char* args, char parentIndex);
void removebyIndex(char index, char* files[512], char* sectors[512], char* maps[512]);
void remove(int argc, char* args[], char parentIndex);
void copy(int argc, char* args[], char parentIndex);
#endif