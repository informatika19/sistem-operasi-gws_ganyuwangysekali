#include "io.h"
#include "kernel.h"
#include "math.h"
extern int row, col;

void printString(char *string){
	int i;
	for(i = 0; string[i] != 0; i++){
		// tulis string :D
		if(string[i] != '\n' && string[i] != '\b')
			interrupt(0x10, 0x0900 | string[i], 0x000F, 0x0001, 0x0000);
		// update posisi kursor
		if(string[i] == '\b') col--;
		else col++;
		if(col > TEXT_WIDTH || string[i] == '\n'){
			col = 0;
			row++;
			if(row == TEXT_HEIGHT){
				interrupt(0x10, 0x0601, 0, 0, TEXT_HEIGHT<<8|TEXT_WIDTH);
				row--;
			}
		}
		// set posisi kursor
		interrupt(0x10, 0x0200, 0x0000, 0x0000, (row<<8)|col);
		if(string[i] == '\b')
			interrupt(0x10, 0x0900, 0x000F, 0x0001, 0x0000);
	}
}

void readString(char *string){
	int length = 0;
	char current = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	while(length < bufsize-1 && current != 0x0D){
		if(current == '\b'){
			if(length > 0) string[length--] = 0;
			printString("\b");
		}
		else{
			string[length] = current;
			printString(string+length);
			length++;
		}
		current = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	}
	if(current == 0x0D && length < bufsize-1){
		string[length] = '\n';
		printString(string+(length++));
	}
}

void printInt(int x){
  char current[2];
  int y, i;
  clear(current, 2);
  if(x < 0) printString("-");
  x = abs(x);
  while(x > 9){
    y = x;
    i = 0;
    while(y > 9) { y/=10; i++; }
    current[0] = '0'+(y&0xFF);
    printString(current);
    x -= y*(pow(10,i));
  }
  current[0] = '0'+(x&0xFF);
  printString(current);
}

void printHex(unsigned int x){
  char current[2];
  unsigned int y, i;
  printString("0x");
  while(x > 0xF){
    y = x;
    i = 0;
    while(y > 0xF) { y /= 0x10; i++; }
    y &= 0xF;
    current[0] = ( y < 10 ? '0' : 'A'-10 )+(y);
    printString(current);
    x -= y*pow(0x10, i);
  }
  x &= 0xF;
  current[0] = ( x < 10 ? '0' : 'A'-10 )+(x);
  printString(current);
}