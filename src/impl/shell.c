#include "shell.h"
#include "file.h"
#include "progs.h"

void runShell() {
    unsigned char arrowClick;
    char command[512], path, lsContent[32], changedPath, commandHistory[8], cmd[8];
    char prompt[100];
    int errNo, historyCount, historyIdx, i;


    strcpy(prompt, "GanyuWangySekali:\\w$ ");
    path = 0xFF;
    arrowClick = 0;
    historyCount = 0;
    historyIdx = -1;
    while (1) {
        if (arrowClick == 0) {
            printPrompt(prompt, path);
        }

        clear(command, 512);
        readString(command);
        command[strlen(command)-1] = 0;
        i = 0;
        while(command[i++] != ' ') cmd[i-1] = command[i-1];

        // if (command[0] == 0x00 && command[1] != 0x00) {
        //     // ini ke atas yak
        //     if (command[1] == 0x38) {
        //         historyIdx--;
        //     }
        //     // ini ke bawah
        //     else if (command[1] == 0x40) {
        //         historyIdx++;
        //     }

        //     if (historyIdx >= 0 && historyIdx < historyCount) {
        //         printString(commandHistory[historyIdx]);
        //     }

        //     arrowClick = 1;
        // }

        // else {
            arrowClick = 0;
            if (strncmp(cmd, "cd", 2) == 0) {
                changedPath = chdir(command+2, &errNo, path);
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

            else if (strncmp(cmd, "ls", 2) == 0) {
                ls(path);
            }

            else if (strncmp(cmd, "cat", 3) == 0) {
                cat(command+3 ,path);
            }

            else if (strncmp(cmd, "ln", 2) == 0) {
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
            // printString("\n");

            // // simpen history
            // for (i = historyCount-1; i > 0; i--) {
            //     strcpy(commandHistory[i], commandHistory[i-1]);
            // }
            // strcpy(commandHistory[historyCount], command);
            // historyCount++;
            // historyIdx = historyCount-1;
        // }
    }
}

void printPath(char path, unsigned char trunc) {
    char order[32], dir[1024], filename[15], currDir[32];
    int numOfOrder, j, i;

    if(path != 0xFF){
        readSector(dir, 0x101);
        readSector(dir+512, 0x102);
    }

    if(!trunc || path == 0xFF) printString("/");
    if(trunc && path != 0xFF){
        clear(filename, 15);
        strncpy(filename, dir+path+2, 14);
        printString(filename);
    }
    else{
        numOfOrder = 0;
        while (path != 0xFF) {
            order[numOfOrder] = path;
            path = dir[path*16];
            numOfOrder++;
        }

        for (i = numOfOrder-1; i >= 0; i--) {
            j = numOfOrder-1-i;
            currDir[j] = order[i];
            clear(filename, 15);
            strncpy(filename, dir+(order[i]<<4)+2, 14);
            printString(filename);
            printString("/");
        }
    }
}

void printPrompt(char* prompt, char dirIdx){
    int i;
    char current[20];
    while(*prompt != 0){
        if(*prompt == '\\'){
            switch(*(++prompt)){
                case 'W':
                    printPath(dirIdx, 1);
                    prompt++;
                    break;
                case 'w':
                    printPath(dirIdx, 0);
                    prompt++;
                    break;
            }
        }
        else{
            i = 0;
            while(*(prompt+i) != 0 && *(prompt+i) != '\\'){
                i++;
            }
            strncpy(current, prompt, i);
            current[i] = 0;
            printString(current);
            prompt += i;
        }
    }
}