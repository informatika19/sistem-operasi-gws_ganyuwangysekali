#include "io.h"
#include "kernel.h"
#include "math.h"

int col = 0, row = 0;

void setCursorPos(int x, int y){
	col = x;
	row = y;
}

void printString(char *string){
	int i;
	for(i = 0; string[i] != 0; i++){
		// tulis string :D
		if(string[i] != '\n' && string[i] != '\b')
			interrupt(0x10, 0x0900 | string[i], 0x000F, 0x0001, 0x0000);
		// update posisi kursor
		if(string[i] == '\b') col--;
		else col++;
		if(col < 0){
			col = TEXT_WIDTH;
			row--;
			if(row < 0){
				interrupt(0x10, 0x0701, 0, 0, TEXT_HEIGHT<<8|TEXT_WIDTH);
				row = 0;
			}
		}
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
	char current;
	int c = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
	current = (char)(c&0xFF);
	while(length < bufsize-1 && current != 0x0D){
		if(current == '\b'){
			if(length > 0){
				string[length--] = 0;
				printString("\b");
			}
		}
		else if(current == 0){ // non-printable, extra keys
			string[length++] = current;
			string[length++] = (char)(c>>8);
			break;
		}
		else{
			string[length] = current;
			printString(string+length);
			length++;
		}
		c = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
		current = (char)(c&0xFF);
	}
	if(current == 0x0D && length < bufsize-1){
		string[length] = '\n';
		printString(string+(length++));
	}
}
