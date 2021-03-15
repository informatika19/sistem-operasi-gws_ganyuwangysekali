#include "shell.h"
#include "file.h"
#include "progs.h"

void runShell() {
    char command[512], path, lsContent[32], changedPath;
    int errNo;

    path = 0xFF;
    while (1) {
        printString("[GanyuWangySekali]> ");
        printPath(path);
        readString(command);

        if (strncmp(command, "cd ", 3) == 1) {
            changedPath = chdir(command+3, &errNo, path);
            if (errNo == 1) {
                printString("Not a directory");
            }
            else if (errNo == 2) {
                printString("No such file or directory");
            }
            else if (errNo == 3) {
                printString("Not a valid file or directory");
            }
            else {
                path = changedPath;
            }
        }

        else if (strncmp(command, "ls", 2) == 1) {
            ls(lsContent, path);
            printString(lsContent);
        }

        else if (strncmp(command, "cat ", 4) == 1) {
            cat(command+3 ,path);
        }

        else if (strncmp(command, "ln ", 3) == 1) {
            if (strncmp(command+3, "-s ", 3)) {
                // softln(command+6, , path);
            }
            else {
                // ln (command+3, , path);
            }
        }

        else {
            printString("No command found");
        }
        printString("\n");
    }
}

void printPath(char path) {
    char order[32], dir[1024], filename[14], currDir[32];
    int numOfOrder, j;

    printString("~");
    if (path != 0xFF) {
        readSector(dir, 0x101);
        readSector(dir+512, 0x102);

        numOfOrder = 0;
        while (path != 0xFF) {
            order[numOfOrder] = path;
            path = dir[path*16];
            numOfOrder++;
        }

        for (int i = numOfOrder-1; i >= 0; i--) {
            j = numOfOrder-1-i;
            currDir[j] = order[i];

            if (i != 0) {
                currDir[j] = strncat(currDir[j], "/", 1);
            }
        }
        printString(currDir);
    }
    printString("$");
}