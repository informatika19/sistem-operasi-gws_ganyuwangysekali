#include "basicio.h"

void readInput(char* buffer){
  interrupt(0x21, 0x0001, buffer, 0, 0);
}

void writeInput(char* buffer) {
  char al, ah;
  int ax, i = 0;
  while (*(buffer + i) != '\0') {
    al = *(buffer + i);
    ah = 0xe;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0, 0, 0);
    if (al == 13) {
      interrupt(0x10, 0xe*0x100+10, 0, 0, 0);
    }
    i++;
  }
}