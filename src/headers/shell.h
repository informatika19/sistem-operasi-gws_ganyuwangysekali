#ifndef SHELL_H
#define SHELL_H

void runShell();

void printPath(char path, unsigned char trunc);
void printPrompt(char* prompt, char dirIdx);
void executeProgram(char *filename, int segment, int *success, char parentIndex);

#endif
