#include "io.h"
#include "kernel.h"
#include "math.h"

void printString(char *string){
	int i;
	for(i = 0; string[i] != 0; i++){
		// tulis string :D
		if(string[i] == '\b'){
			printBackspace();
		}
		else{
			interrupt(0x10, 0x0E00 | string[i], 0x0F, 0, 0);
			if(string[i] == '\n') interrupt(0x10, 0x0E0d, 0x00, 0, 0);
		}
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
				printBackspace();
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
