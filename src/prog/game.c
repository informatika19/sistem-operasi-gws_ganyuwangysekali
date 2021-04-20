#include "fileio.h"
#include "buffer.h"
#include "string.h"

void game();

int main() {
    char buffer[512 << 4];
    char parent, arg[512];
    int err;

    lib_readFile(buffer, "tempc", &err, 0xFF);
    removeFEntry("tempc", 0xFF, &err);
    parse(buffer, &parent, arg);

    game(arg, parent);

    clear(buffer, 8192);
    buffer[0] = parent;
    err = 1;
    lib_writeFile(buffer, "tempc", &err, 0xFF);

    exec("/bin/shell", 0xFF, &err);
}

void game() {
    char choice[20];
    print("Choose rock/paper/scissors: ");
    readInput(choice);
    if (strncmp(choice, "rock", 4) == 0) {
        print("We choose paper. You lose!\n");
    }
    else if (strncmp(choice, "paper", 5) == 0) {
        print("We choose scissors. You lose!\n");
    }
    else if (strncmp(choice, "scissors", 8) == 0) {
        print("We choose rock. You lose!\n");
    }
    else {
        print("What did you just type? I guess you win this round\n");
    }
}