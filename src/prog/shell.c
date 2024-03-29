#include "fileio.h"
#include "buffer.h"

void printPrompt();

int main() {
    unsigned char arrowClick;
    char command[512], path, changedPath, commandHistory[4][512], cmd[256];
    char fileBuf[8192];
    char prompt[100];
    int errNo, historyCount, historyIdx, i;

    clear(prompt, 100);
    lib_readFile(fileBuf, "history", &errNo, 0xFF);
    if(errNo > 0){
        historyCount = (int)fileBuf[2048];
        for(i = 0; i < 4; i++){
            for(errNo = 0; errNo < 512; errNo++){
                commandHistory[i][errNo] = fileBuf[i*512+errNo];
            }
        }
    }
    else{
        historyCount = 0;
    }
    strcpy(prompt, "GanyuWangySekali:\\w$ ");
    
    lib_readFile(fileBuf, "tempc", &errNo, 0xFF);
    if(errNo > 0)
        path = fileBuf[0];
    else
        path = 0xFF;
    removeFEntry("tempc", 0xFF, &errNo);
    arrowClick = 0;
    historyIdx = -1;
    printPrompt(prompt, path);
    while (1) {
        if(arrowClick == 0){
            clear(command, 512);
            command[0] = path;
            command[1] = 0;
        }
        clear(cmd, 256);
        readInput(command+strlen(command));
        if(command[strlen(command)-1] == '\n') command[strlen(command)-1] = 0;
        i = 1;
        while(command[i] != 0) i++;

        if (command[i] == 0x00 && command[i+1] != 0) { // maybe special command?
            // ini ke atas yak
            errNo = strlen(commandHistory[historyIdx]);
            if (command[i+1] == 0x48) {
                historyIdx++;
            }
            // ini ke bawah
            else if (command[i+1] == 0x50) {
                historyIdx--;
            }

            if(historyIdx < 0){
                historyIdx = -1;
                arrowClick = 0;
            }
            else if(historyIdx > historyCount){
                historyIdx = historyCount;
            }
            if(historyIdx >= 0 && historyIdx < historyCount){
                for(i = 0; i < errNo; i++) print("\b"); // hapus history sebelumnya
                clear(command, 512);
                strcpy(command, commandHistory[historyIdx]);
                print(commandHistory[historyIdx]+1);
                arrowClick = 1;
            }
        }

        else {
            i = 1;
            while(command[i] == ' ') i++;
            errNo = 0;
            while(command[i] != ' ' && command[i] != 0){
                cmd[errNo] = command[i];
                i++;
                errNo++;
            }
            cmd[errNo] = 0;
            command[0] = path;
            if(strncmp(cmd, "") == 0) exec("/bin/shell", 0xFF, &errNo);
            // simpen history
            historyCount++;
            if(historyCount > 4) historyCount = 4;
            historyIdx = -1;
            for (i = historyCount-1; i > 0; i--) {
                strcpy(commandHistory[i], commandHistory[i-1]);
            }
            strcpy(commandHistory, command);
            for(i = 0; i < 4; i++){
                for(errNo = 0; errNo < 512; errNo++){
                    fileBuf[(i*512)+errNo] = commandHistory[i][errNo];
                }
            }
            *(fileBuf+2048) = (historyCount&0xFF);
            removeFEntry("history", 0xFF, &errNo);
            errNo = 5;
            lib_writeFile(fileBuf, "history", &errNo, 0xFF);
            errNo = 1;
            lib_writeFile(command, "tempc", &errNo, 0xFF);
            if(errNo>0){
                if(strncmp(cmd, "./", 2) == 0){
                    exec(cmd+2, path, &errNo);
                }
                else{
                    exec(cmd, getParent("bin", 0xFF), &errNo);
                }
            }
            print("Invalid command!\n");
            exec("/bin/shell", 0xFF, &errNo);
        }
    }
    return;
}

void printPath(char path, unsigned char trunc) {
    char order[32], dir[1024], filename[15], currDir[32];
    int numOfOrder, j, i;

    if(path != 0xFF){
        lib_readSector(dir, 0x101);
        lib_readSector(dir+512, 0x102);
    }

    if(!trunc || path == 0xFF) print("/");
    if(trunc && path != 0xFF){
        clear(filename, 15);
        strncpy(filename, dir+path+2, 14);
        print(filename);
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
            print(filename);
            print("/");
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
            print(current);
            prompt += i;
        }
    }
}