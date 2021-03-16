#include "shell.h"
#include "file.h"
#include "progs.h"

void runShell() {
    char command[512], lsContent[32], path, changedPath, commandHistory[8];
    int errNo, arrowClick, historyCount, historyIdx, i;
    
    path = 0xFF;
    arrowClick = 0;
    historyCount = 0;
    historyIdx = -1;
    while (1) {
        if (arrowClick == 0) {
            printString("[GanyuWangySekali]> ");
            printPath(path);
        }

        readString(command);

        if (command[0] == 0x00 && command[1] != 0x00) {
            // ini ke atas yak
            if (command[1] == 0x38) {
                historyIdx--;
            }
            // ini ke bawah
            else if (command[1] == 0x40) {
                historyIdx++;
            }

            if (historyIdx >= 0 && historyIdx < historyCount) {
                printString(commandHistory[historyIdx]);
            }

            arrowClick = 1;
        }

        else {
            arrowClick = 0;
            if (strncmp(command, "cd ", 3) == 0) {
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

            else if (strncmp(command, "ls", 2) == 0) {
                ls(lsContent, path);
                printString(lsContent);
            }

            else if (strncmp(command, "cat ", 4) == 0) {
                cat(command+3 ,path);
            }

            else if (strncmp(command, "ln ", 3) == 0) {
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

            // simpen history
            for (i = historyCount-1; i > 0; i--) {
                strcpy(commandHistory[i], commandHistory[i-1]);
            }
            strcpy(commandHistory[historyCount], command);
            historyCount++;
            historyIdx = historyCount-1;
        }
    }
}

void printPath(char path) {
    char order[32], dir[1024], filename[14], currDir[32];
    int numOfOrder, j, i;

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

        for (i = numOfOrder-1; i >= 0; i--) {
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
