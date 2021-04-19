#include "buffer.h"

void clear(char* buffer, int length){
	int i;
	for(i = 0; i < length; i++){
		buffer[i] = 0;
	}
}

void parse(char* cmd, char* parent, char* arg){
	*parent = *cmd;
	cmd++;
	
	while(*cmd == ' ') cmd++; // ignore leading spaces
	while(*cmd != ' ' && *cmd != 0) cmd++; // ignore arg[0]
	while(*cmd == ' ') cmd++; // ignore space between arg[0] and arg[1]
	// copy arg[1] to arg
	while(*cmd != 0){
		*arg = *cmd;
		arg++;
		cmd++;
	}
	*arg = 0;
}